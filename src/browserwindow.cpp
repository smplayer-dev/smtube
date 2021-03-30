/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015-2021 Ricardo Villalba <ricardo@smplayer.info>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "browserwindow.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QNetworkProxyFactory>
#include <QProcess>
#include <QSettings>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopServices>
#include <QWebFrame>

#include "mywebview.h"
#include "mywebpage.h"
#include "mycookiejar.h"

#include "supportedurls.h"
#include "retrieveyoutubeurl.h"

#include "configdialog.h"
#include "about.h"
#include "version.h"
#include "links.h"
#include "desktopinfo.h"
#include "qtcompat.h"

#ifdef CODEDOWNLOADER
#include "codedownloader.h"
#endif

#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#include "myscroller.h"
#else
#include <QDesktopServices>
#endif

BrowserWindow::BrowserWindow(const QString & config_path, QWidget * parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
	, preferred_resolution(RetrieveYoutubeUrl::R360p)
	, use_cookies(true)
	, current_player(Undefined)
#ifdef D_BUTTON
	, add_download_button(false)
#endif
{
	setWindowTitle("SMTube");
	setWindowIcon(QPixmap(":/icons/smtube.png"));

	settings = new QSettings(config_path + "/smtube2.ini", QSettings::IniFormat, this);

	ryu = new RetrieveYoutubeUrl(this);
	connect(ryu, SIGNAL(gotPreferredUrl(const QString &, int)), this, SLOT(openYTUrl(const QString &, int)));
	connect(ryu, SIGNAL(gotEmptyList()), this, SLOT(showErrorEmptyList()));
	connect(ryu, SIGNAL(processFailedToStart()), this, SLOT(YTFailedToStart()));

	ryua = new RetrieveYoutubeUrl(this);
	ryua->setUseDASH(true);
	connect(ryua, SIGNAL(gotPreferredUrl(const QString &, int)), this, SLOT(openYTAudioUrl(const QString &, int)));
	connect(ryua, SIGNAL(gotEmptyList()), this, SLOT(showErrorEmptyList()));
	connect(ryua, SIGNAL(processFailedToStart()), this, SLOT(YTFailedToStart()));

	QNetworkProxyFactory::setUseSystemConfiguration(true);

	MyWebPage * page = new MyWebPage(this);

	view = new MyWebView(this);
	connect(view, SIGNAL(requestedOpenWith(const QString&, const QUrl&)), this, SLOT(openWith(const QString&, const QUrl&)));
	connect(view, SIGNAL(requestedOpenWithBrowser(const QUrl&)), this, SLOT(openWithBrowser(const QUrl&)));
	connect(view, SIGNAL(requestedOpenAudioWith(const QString&, const QUrl&)), this, SLOT(openAudioWith(const QString&, const QUrl&)));

	view->setPage(page);
	view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

#if QT_VERSION >= 0x050000
	//MyScroller::setScroller(view);
#endif

	connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
	connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
	connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
	connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
	connect(view->page(), SIGNAL(linkClicked(const QUrl&)), this, SLOT(processLink(const QUrl&)));
	//connect(view, SIGNAL(statusBarMessage(const QString &)), statusBar(), SLOT(showMessage(const QString &)));
	connect(view->page(), SIGNAL(linkHovered(const QString &, const QString &, const QString &)), 
            statusBar(), SLOT(showMessage(const QString &)));

	location = new QLineEdit(this);
	location->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	connect(location, SIGNAL(returnPressed()), SLOT(changeLocation()));

	QAction * loadHomePageAct = new QAction(QPixmap(":/icons/go-home.png"), tr("Home"), this);
	connect(loadHomePageAct, SIGNAL(triggered()), SLOT(loadHomePage()));

	toolbar = addToolBar(tr("Navigation"));
	toolbar->addAction(loadHomePageAct);
	toolbar->addAction(view->pageAction(QWebPage::Back));
	toolbar->addAction(view->pageAction(QWebPage::Forward));
	toolbar->addAction(view->pageAction(QWebPage::Reload));
	toolbar->addAction(view->pageAction(QWebPage::Stop));
	toolbar->addWidget(location);

	QAction * incFontAct = new QAction(tr("Zoom +"), this);
	incFontAct->setShortcut(QKeySequence::ZoomIn);
	connect(incFontAct, SIGNAL(triggered()), this, SLOT(incZoom()));

	QAction * decFontAct = new QAction(tr("Zoom -"), this);
	decFontAct->setShortcut(QKeySequence::ZoomOut);
	connect(decFontAct, SIGNAL(triggered()), this, SLOT(decZoom()));

	QAction * quitAct = new QAction(tr("&Quit"), this);
	connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

	QMenu * browseMenu = menuBar()->addMenu(tr("&Navigation"));
	browseMenu->addAction(loadHomePageAct);
	browseMenu->addAction(view->pageAction(QWebPage::Back));
	browseMenu->addAction(view->pageAction(QWebPage::Forward));
	browseMenu->addAction(view->pageAction(QWebPage::Reload));
	browseMenu->addAction(view->pageAction(QWebPage::Stop));
	browseMenu->addSeparator();
	browseMenu->addAction(incFontAct);
	browseMenu->addAction(decFontAct);
	browseMenu->addSeparator();
	browseMenu->addAction(quitAct);

	QMenu * viewMenu = menuBar()->addMenu(tr("&View"));
	toggleToolbarAct = new QAction(tr("Toolbar"), this);
	toggleToolbarAct->setCheckable(true);
	connect(toggleToolbarAct, SIGNAL(toggled(bool)), this, SLOT(viewToolbar(bool)));

	toggleStatusbarAct = new QAction(tr("Statusbar"), this);
	toggleStatusbarAct->setCheckable(true);
	connect(toggleStatusbarAct, SIGNAL(toggled(bool)), this, SLOT(viewStatusbar(bool)));

	QAction * showConfigDialogAct = new QAction(tr("&Settings"), this);
	connect(showConfigDialogAct, SIGNAL(triggered()), this, SLOT(showConfigDialog()));

	viewMenu->addAction(toggleToolbarAct);
	viewMenu->addAction(toggleStatusbarAct);
	viewMenu->addSeparator();
	viewMenu->addAction(showConfigDialogAct);

	QMenu * helpMenu = menuBar()->addMenu(tr("&Help"));

#ifdef CODEDOWNLOADER
	#if defined(Q_OS_WIN) && !defined(PORTABLE_APP)
	QAction * updateCodeAct = new QAction(tr("&Update YouTube playback support"), this);
	#else
	QAction * updateCodeAct = new QAction(tr("&Install / Update YouTube playback support"), this);
	#endif
	connect(updateCodeAct, SIGNAL(triggered()), this, SLOT(updateYTCode()));
	helpMenu->addAction(updateCodeAct);
#endif

	QAction * aboutAct = new QAction(tr("&About SMTube"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(showAbout()));
	helpMenu->addAction(aboutAct);

	setCentralWidget(view);

	//menuBar()->hide();
	toolbar->hide();
	statusBar()->hide();

#ifdef STYLE_SWITCHING
	default_style = qApp->style()->objectName();
#endif

	loadConfig();

	if (use_cookies) {
		MyCookieJar * jar = new MyCookieJar(config_path + "/cookies.ini");
		view->page()->networkAccessManager()->setCookieJar(jar);
	}
}

BrowserWindow::~BrowserWindow() {
	saveConfig();
}

QString BrowserWindow::homePageUrl() {
#ifdef USE_SITES
	return sites.currentSite().homeUrl();
#else
	return "http://www.tonvid.com/";
#endif
}

void BrowserWindow::viewToolbar(bool b) {
	qDebug() << "BrowserWindow::viewToolbar:" << b;
	toolbar->setVisible(b);
//	toggleToolbarAct->setChecked(b);
}

void BrowserWindow::viewStatusbar(bool b) {
	qDebug() << "BrowserWindow::viewStatusbar:" << b;
	statusBar()->setVisible(b);
//	toggleStatusbarAct->setChecked(b);
}

void BrowserWindow::incZoom() {
	qreal z = view->page()->mainFrame()->zoomFactor();
	view->page()->mainFrame()->setZoomFactor(z + 0.1);
}

void BrowserWindow::decZoom() {
	qreal z = view->page()->mainFrame()->zoomFactor();
	view->page()->mainFrame()->setZoomFactor(z - 0.1);
}

void BrowserWindow::loadUrl(const QUrl & url) {
	view->load(url);
}

void BrowserWindow::search(const QString & term) {
	QString search_term = term.toLower();
	if (search_term.startsWith("http://") || search_term.startsWith("https://")) {
		search_term = search_term.replace("http://", "");
		search_term = search_term.replace("https://", "");
	}

#ifdef USE_SITES
	QString q = sites.currentSite().searchUrl();
	q.replace("SEARCHTERM", search_term);
#else
	QString q = homePageUrl() + "search.php?q=" + search_term;
#endif
	loadUrl(QUrl(q));
}

void BrowserWindow::adjustLocation() {
	QString u = view->url().toString();
	qDebug() << "BrowserWindow::adjustLocation:" << u;
	location->setText(u);
}

void BrowserWindow::changeLocation() {
	QString text = location->text();
	qDebug() << "BrowserWindow::changeLocation:" << text;

#if 1
	view->load(QUrl::fromUserInput(text));
#else
	QRegExp rx("((https?:)?//)?(([\\d\\w]|%[a-fA-f\\d]{2,2})+(:([\\d\\w]|%[a-fA-f\\d]{2,2})+)?@)?([\\d\\w][-\\d\\w]{0,253}[\\d\\w]\\.)+[\\w]{2,63}(:[\\d]+)?(/([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})*)*(\\?(&?([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})=?)*)?(#([-+_~.\\d\\w]|%[a-fA-f\\d]{2,2})*)?");
	bool valid = (rx.indexIn(text) > -1);
	qDebug() << "BrowserWindow::changeLocation: url valid:" << valid;

	if (!valid) {
		search(text);
	} else {
		view->load(QUrl::fromUserInput(text));
	}
#endif
	view->setFocus();
}

void BrowserWindow::adjustTitle() {
	setWindowTitle(view->title());
}

void BrowserWindow::setProgress(int p) {
	statusBar()->showMessage(tr("Loading... %1%").arg(p));
}

void BrowserWindow::finishLoading(bool) {
	statusBar()->clearMessage();

#ifdef D_BUTTON
	QString code;
	QString url = view->url().toString();

	qDebug() << "BrowserWindow::finishLoading: url:" << url;
	qDebug() << "BrowserWindow::finishLoading: add_download_button:" << add_download_button;
	qDebug() << "BrowserWindow::finishLoading: external_download_url:" << external_download_url;

	QString title_txt = tr("Download video using an external service").replace("'", "\\'");
	QString download_txt = tr("Download").replace("'", "\\'");

	if (add_download_button && url.contains("/info.php?") &&
		(external_download_url.contains("%YT_URL%") || external_download_url.contains("%YT_ID%")))
	{
		code =	"var video_url = document.getElementById('video_thumbnail').href;"
				"if (video_url) {"
					"var video_id = document.getElementById('video_thumbnail').dataset.id;"
					"var div = document.getElementById('published');"
					"var link = '" + external_download_url +"';"
					"link = link.replace('%YT_URL%', video_url);"
					"if (video_id) link = link.replace('%YT_ID%', video_id);"
					"div.cells[0].colSpan = 5;"
					"div.innerHTML = div.innerHTML + "
					"'<td><a target=\"_blank\" class=\"btn btn-primary btn-xs\" "
					"title=\"" + title_txt +"\" "
					"href=\"' + link + '\">"
					"<span class=\"glyphicon glyphicon-download\"></span> " + download_txt +"</a></td>';"
				"}";

		view->page()->mainFrame()->evaluateJavaScript(code);
	}
#endif
}

void BrowserWindow::processLink(const QUrl & url ) {
	qDebug() << "BrowserWindow::processLink:" << url.toString();

	QString link = url.toString();

	bool can_play_this = false;
	int site_type = SupportedUrls::site(url.toString());

	#ifdef USE_PLAYERS
	// Try to play with the first player on the list
	int player_id = 0;
	QList<Player> pl = players.availablePlayers();
	if (pl.count() > 0) {
		if (site_type != SupportedUrls::Unsupported) {
			can_play_this = true;
		}
	}
	#else
	if (site_type != SupportedUrls::Unsupported) {
		can_play_this = true;
	}
	#endif

	if (!can_play_this) {
		if (view->lastClickedUrl() == url && view->lastClickedTarget() == "_blank") {
			qDebug() << "BrowserWindow::processLink: link will be opened in an external browser";
			QDesktopServices::openUrl(url);
		} else {
			view->load(url);
		}
	} else {
		#ifdef USE_PLAYERS
		openWith(pl[player_id].name(), QUrl(link));
		#else
		openWith(0, QUrl(link));
		#endif
	}
}

void BrowserWindow::fetchVideoUrl(RetrieveYoutubeUrl * ry, const QUrl & url, int player_id) {
	current_player = player_id;
	int resolution = preferred_resolution;

	#ifdef USE_PLAYERS
	if (player_id > -1) {
		if (players.item(player_id).preferredResolution() != -1) {
			resolution = players.item(player_id).preferredResolution();
		}
	}
	#endif

	ry->setPreferredResolution((RetrieveYoutubeUrl::Resolution) resolution);
	ry->fetchPage(url.toString());
}

void BrowserWindow::openWith(int player_id, const QUrl & url) {
	qDebug() << "BrowserWindow::openWith: player_id:" << player_id << "url:" << url.toString();

#ifdef USE_PLAYERS
	QString binary = players.item(player_id).executable();
	QString player_name = players.item(player_id).name();
	QString arguments = players.item(player_id).arguments();
	bool support_streaming_sites = players.item(player_id).supportStreamingSites();
	/* bool support_online_tv = players.item(player_id).supportOnlineTV(); */
#else
	QString binary = HCPLAYER_EXECUTABLE;
	QString player_name = HCPLAYER_NAME;
	QString arguments = HCPLAYER_ARGUMENTS;
	bool support_streaming_sites = HCPLAYER_STREAMINGSITES;
#endif

	int site_type = SupportedUrls::site(url.toString());
	bool direct_play = true;
	if (site_type == SupportedUrls::Youtube && !support_streaming_sites) direct_play = false;

	if (direct_play) {
		qDebug() << "BrowserWindow::openWith: ready to play with" << player_name;
		QStringList arg_list = arguments.split(' ', QTC_SkipEmptyParts);
		QString expanded_arguments;
		for (int n = 0; n < arg_list.count(); n++) {
			arg_list[n] = arg_list[n].replace("%u", url.toString());
			expanded_arguments += " "+ arg_list[n];
		}
		qDebug() << "BrowserWindow::openWith: command:" << binary + expanded_arguments;
		QProcess::startDetached(binary, arg_list);
	} else {
		qDebug() << "BrowserWindow::openWith:" << player_name << "can't play this URL";
		#ifdef USE_PLAYERS
		fetchVideoUrl(ryu, url, player_id);
		#else
		fetchVideoUrl(ryu, url);
		#endif
	}
}

void BrowserWindow::openWith(const QString & player, const QUrl & url) {
	qDebug() << "BrowserWindow::openWith: player:" << player << "url:" << url.toString();

#ifdef USE_PLAYERS
	int p = players.findName(player);
	if (p != -1) {
		openWith(p, url);
	} else {
		qWarning() << "BrowserWindow::openWith: player not found";
	}
#else
	openWith(0, url);
#endif
}

void BrowserWindow::openWithBrowser(const QUrl & url) {
	qDebug() << "BrowserWindow::openWithBrowser: url:" << url.toString();
	fetchVideoUrl(ryu, url, WebBrowser);
}

void BrowserWindow::openYTUrl(QString title, QString extension, const QString & url) {
	qDebug() << "BrowserWindow::openYTUrl:" << url;

	if (current_player == WebBrowser) {
		QString u = url;
		#if 1
		u = "http://www.tonvid.com/video.php?u=" + url.toUtf8().toBase64().toPercentEncoding() +
            "&t=" + title.toUtf8().toBase64().toPercentEncoding();
		#endif
		QDesktopServices::openUrl(u);
		return;
	}

#ifdef USE_PLAYERS
	if (current_player == -1) {
		qDebug() << "BrowserWindow::openYTUrl: invalid player";
		return;
	}
#endif

	title = title.replace('"', "'");

	QString filename = title;
	filename = filename.replace(" - YouTube", "").replace(":", "").replace("|","");
	filename += extension;

	qDebug() << "BrowserWindow::openYTUrl: filename:" << filename;

#ifdef USE_PLAYERS
	QString binary = players.item(current_player).executable();
	QString arguments = players.item(current_player).arguments();
#else
	QString binary = HCPLAYER_EXECUTABLE;
	QString arguments = HCPLAYER_ARGUMENTS;
#endif

	QStringList arg_list = arguments.split(' ', QTC_SkipEmptyParts);
	QString expanded_arguments;
	for (int n = 0; n < arg_list.count(); n++) {
		if (arg_list[n].contains("%u")) {
			arg_list[n] = arg_list[n].replace("%u", url);
		} else {
			arg_list[n] = arg_list[n].replace("%f", filename).replace("%t", title);
		}
		expanded_arguments += " "+ arg_list[n];
	}

	qDebug() << "BrowserWindow::openYTUrl: command:" << binary + expanded_arguments;
	QProcess::startDetached(binary, arg_list);
}

void BrowserWindow::openYTUrl(const QString & url, int itag) {
	openYTUrl(ryu->videoTitle(), ryu->extensionForItag(itag), url);
}

void BrowserWindow::openAudioWith(const QString & player, const QUrl & url) {
	qDebug() << "BrowserWindow::openAudioWith: player:" << player << "url:" << url.toString();

#ifdef USE_PLAYERS
	int p = players.findName(player);
#else
	int p = 0;
#endif

	if (p != -1) {
		#ifdef USE_PLAYERS
		fetchVideoUrl(ryua, url, p);
		#else
		fetchVideoUrl(ryua, url);
		#endif
	}
}

void BrowserWindow::openYTAudioUrl(const QString &, int) {
	qDebug() << "BrowserWindow::openYTAudioUrl";

	int itag = ryua->selectedAudioQuality();
	QString url = ryua->selectedAudioUrl();

	qDebug() << "BrowserWindow::openYTAudioUrl: url:" << url << "itag:" << itag;

	if (!url.isEmpty()) {
		openYTUrl(ryua->videoTitle(), ryu->extensionForItag(itag), url);
	}
}

void BrowserWindow::showErrorEmptyList() {
	qDebug() << "showErrorEmptyList";

#ifdef CODEDOWNLOADER
	CodeDownloader::askAndDownload(this, CodeDownloader::UrlNotFound);
#else
	QMessageBox::warning(this, tr("No video found"),
		tr("It wasn't possible to find the URL for this video."));
#endif
}

#ifdef CODEDOWNLOADER
void BrowserWindow::updateYTCode() {
	qDebug() << "BrowserWindow::updateYTCode";
	CodeDownloader::askAndDownload(this);
}
#endif

void BrowserWindow::YTFailedToStart() {
	CodeDownloader::askAndDownload(this, CodeDownloader::FailedToRun);
}

void BrowserWindow::showAbout() {
	About d(this);
	d.exec();
}

void BrowserWindow::showConfigDialog() {
	ConfigDialog d(this);

	d.setPlaybackResolution(preferred_resolution);
	#ifdef USE_PLAYERS
	d.setPlayers(players.allPlayers());
	d.setDefaultPlayers(players.defaultPlayers());
	#endif

	#ifdef USE_SITES
	d.setSites(sites.allSites());
	d.setCurrentSite(sites.current());
	bool needs_refresh = false;
	#endif

	#ifdef D_BUTTON
	d.setAddDownloadButton(add_download_button);
	d.setExternalDownloadUrl(external_download_url);
	#endif

	d.setDefaultFont(qApp->font());

#ifdef STYLE_SWITCHING
	d.setStyle(qApp->style()->objectName());
#endif

	if (d.exec() == QDialog::Accepted) {
		preferred_resolution = d.playbackResolution();
		#ifdef USE_PLAYERS
		players.setAllPlayers(d.players());
		view->setPlayers(players.availablePlayers());
		#endif

		#ifdef USE_SITES
		int current_site = d.currentSite();
		if (current_site != sites.current()) {
			sites.setCurrent(current_site);
			needs_refresh = true;
		}
		sites.setAllSites(d.sites());
		#endif

		#ifdef D_BUTTON
		add_download_button = d.addDownloadButton();
		external_download_url = d.externalDownloadUrl();
		#endif

		QFont new_font = d.defaultFont();
		if (qApp->font() != new_font) qApp->setFont(new_font);

		#ifdef STYLE_SWITCHING
		setStyle(d.style());
		#endif
	}

	#ifdef USE_SITES
	if (needs_refresh) loadHomePage();
	#endif
}

void BrowserWindow::saveConfig() {
	qDebug() << "BrowserWindow::saveConfig";

	settings->beginGroup("window");
	settings->setValue("size", size());
	settings->setValue("pos", pos());
	settings->setValue("font", qApp->font().toString());
#ifdef STYLE_SWITCHING
	settings->setValue("style", qApp->style()->objectName());
#endif
	settings->endGroup();

	settings->beginGroup("view");
	settings->setValue("toolbar", toggleToolbarAct->isChecked());
	settings->setValue("statusbar", toggleStatusbarAct->isChecked());
	settings->endGroup();

	settings->beginGroup("General");
	settings->setValue("playback_resolution", preferred_resolution);
	settings->setValue("user_agent", ryu->userAgent());

#ifdef D_BUTTON
	settings->setValue("add_download_button", add_download_button);
	settings->setValue("external_download_url", external_download_url);
#endif

	settings->endGroup();

	settings->beginGroup("browser");
	QWebSettings * ws = view->page()->settings();
	settings->setValue("plugins", ws->testAttribute(QWebSettings::PluginsEnabled));
	settings->setValue("javascript", ws->testAttribute(QWebSettings::JavascriptEnabled));
	settings->setValue("java", ws->testAttribute(QWebSettings::JavaEnabled));
	settings->setValue("images", ws->testAttribute(QWebSettings::AutoLoadImages));
	settings->setValue("developer_extras", ws->testAttribute(QWebSettings::DeveloperExtrasEnabled));
	settings->setValue("use_cookies", use_cookies);
	settings->setValue("minimum_font_size", ws->fontSize(QWebSettings::MinimumFontSize));
	settings->setValue("zoom", view->page()->mainFrame()->zoomFactor());
	settings->endGroup();

	settings->beginGroup("youtube-dl");
	settings->setValue("ytdl_bin", ytdl_bin);
	settings->setValue("override_format", ryu->userFormat());
	settings->setValue("use_av1", ryu->isAv1Enabled());
	settings->endGroup();

#ifdef USE_PLAYERS
	players.save(settings);
#endif

#ifdef USE_SITES
	sites.save(settings);
#endif
}

void BrowserWindow::loadConfig() {
	qDebug() << "BrowserWindow::loadConfig";

	settings->beginGroup("window");
	resize(settings->value("size", QSize(650, 715)).toSize());
	move(settings->value("pos", pos()).toPoint());
	QFont f;
	QString current_font = qApp->font().toString();
	f.fromString(settings->value("font", current_font).toString());
	qApp->setFont(f);

#ifdef STYLE_SWITCHING
	setStyle(settings->value("style", "").toString());
#endif

	settings->endGroup();

	if (!DesktopInfo::isInsideScreen(this)) {
		move(DesktopInfo::topLeftPrimaryScreen());
		qWarning("BrowserWindow::loadConfig: window is outside of the screen, moved to 0x0");
	}

	settings->beginGroup("view");
	toggleToolbarAct->setChecked(settings->value("toolbar", false).toBool());
	toggleStatusbarAct->setChecked(settings->value("statusbar", false).toBool());
	settings->endGroup();

	settings->beginGroup("General");
	preferred_resolution = settings->value("playback_resolution", RetrieveYoutubeUrl::R360p).toInt();
	ryu->setUserAgent(settings->value("user_agent", "").toString());

#ifdef D_BUTTON
	add_download_button = settings->value("add_download_button", false).toBool();

	QString default_url = "http://www.dlvyoutube.com/%YT_URL%";
	external_download_url = settings->value("external_download_url", default_url).toString();
#endif

	settings->endGroup();

	settings->beginGroup("browser");
	QWebSettings * ws = view->page()->settings();
	ws->setAttribute(QWebSettings::PluginsEnabled, settings->value("plugins", false).toBool());
	ws->setAttribute(QWebSettings::JavascriptEnabled, settings->value("javascript", true).toBool());
	ws->setAttribute(QWebSettings::JavaEnabled, settings->value("java", false).toBool());
	ws->setAttribute(QWebSettings::AutoLoadImages, settings->value("images", true).toBool());
	ws->setAttribute(QWebSettings::DeveloperExtrasEnabled, settings->value("developer_extras", false).toBool());
	#ifndef PORTABLE_APP
	bool use_cache = settings->value("use_cache", true).toBool();
	#endif
	use_cookies = settings->value("use_cookies", use_cookies).toBool();

	int current_font_size = ws->fontSize(QWebSettings::MinimumFontSize);
	int minimum_font_size = settings->value("minimum_font_size", current_font_size).toInt();
	ws->setFontSize(QWebSettings::MinimumFontSize, minimum_font_size);

	qreal zoom = settings->value("zoom", 1).toReal();
	view->page()->mainFrame()->setZoomFactor(zoom);

	settings->endGroup();

	settings->beginGroup("youtube-dl");
	ytdl_bin = settings->value("ytdl_bin", "").toString();
	ryu->setYtdlBin(ytdl_bin);
	ryua->setYtdlBin(ytdl_bin);
	ryu->setUserFormat(settings->value("override_format", "").toString());
	ryu->enableAv1(settings->value("use_av1", false).toBool());
	settings->endGroup();

	#ifndef PORTABLE_APP
	if (use_cache) {
		#if QT_VERSION >= 0x050000
		QString cache_path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
		#else
		QString cache_path = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
		#endif
		qDebug() << "BrowserWindow::loadConfig: cache enabled. Location:" << cache_path;
		QWebSettings::enablePersistentStorage(cache_path);
	}
	#endif

	SupportedUrls::load();

#ifdef USE_PLAYERS
	players.load(settings);
	view->setPlayers(players.availablePlayers());
#else
	view->setPlayer(HCPLAYER_NAME);
#endif

#ifdef USE_SITES
	sites.load(settings);
#endif
}

#ifdef STYLE_SWITCHING
void BrowserWindow::setStyle(QString style) {
	if (style.isEmpty()) style = default_style;
	QString current_style = qApp->style()->objectName();
	if (!style.isEmpty() && style != current_style) {
		qDebug() << "BrowserWindow::setStyle:" << style;
		//qApp->setStyleSheet("");
		qApp->setStyle(style);
		#ifdef Q_OS_WIN
		qApp->setPalette(qApp->style()->standardPalette());
		#endif
	}
}
#endif

#include "moc_browserwindow.cpp"
