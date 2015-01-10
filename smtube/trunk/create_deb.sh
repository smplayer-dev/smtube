#! /bin/sh
 
ln -s debian-rvm debian
cp debian-rvm/changelog-orig debian-rvm/changelog

./get_svn_revision.sh
dch -v 14.12.0-`cat svn_revision` "New version"

#dpkg-buildpackage -rfakeroot

# This should be faster:
rm build-stamp
rm src/smtube
fakeroot debian/rules build
fakeroot debian/rules binary

dh_clean
rm debian-rvm/changelog
rm debian
