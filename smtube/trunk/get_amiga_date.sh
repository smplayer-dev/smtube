#!/bin/sh

if [ -f src/amiga_date.h ]; then
delete src/amiga_date.h
fi

echo -n "#define DATE_AMIGA \"(" >> src/amiga_date.h
date +"%d.%m.%Y)\"" >> src/amiga_date.h


