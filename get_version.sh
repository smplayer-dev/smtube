#! /bin/sh
./get_svn_revision.sh
echo "20.6.0.`cat svn_revision`" > version
