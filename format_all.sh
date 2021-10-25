#!/usr/bin/env bash

###################################################################
#Script Name	:format_all.sh                                                                                           
#Description	:format all files                                                                                   
#Author       	:Vanderson Santos                                                
#Email         	:vanderson.santos@usp.br                                          
###################################################################

HEADER_FILES=$(find . -name "*.h")
SOURCE_FILES=$(find . -name "*.cpp")
ARDUINO_FILES=$(find . -name "*.ino")

uncrustify -c uncrustify.cfg --replace --no-backup $HEADER_FILES

uncrustify -c uncrustify.cfg --replace --no-backup $SOURCE_FILES

uncrustify -c uncrustify.cfg --replace --no-backup $ARDUINO_FILES