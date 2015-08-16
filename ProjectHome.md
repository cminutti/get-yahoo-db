# Description #

By providing a list of symbols and the desired date range, you get a database
with information of adjusted closing prices (optionally: open, close, high,
low prices and volume traded too) of the assets in a single CSV file.



# Usage #

In a terminal run the command (Windows users: _Start>Run>cmd_):
```
getyahoodb dd/mm/yyyy dd/mm/yyyy list  [open close high low volume]
```
where _dd/mm/yyyy_ is the start and end date in day-month-year format.
_'list'_ is a list of symbols separated by _'+'_ character.

You can optionally include other fields in the database, as are the open,
close, high, low prices and volume traded.

After running the command, it generates a [CSV](http://en.wikipedia.org/wiki/Comma-separated_values) file with the name 'output.csv'
containing the requested data. This file can be opened with any spreadsheet
(Excel, LibreOffice Calc, etc.).


**Screenshot: example output file**
![http://get-yahoo-db.googlecode.com/svn/trunk/images/screenshot-1.jpg](http://get-yahoo-db.googlecode.com/svn/trunk/images/screenshot-1.jpg)


# Examples #
```
getyahoodb 01/01/2011 01/01/2012 ^GSPC+^IXIC+^HSI
getyahoodb 01/01/2011 01/01/2012 AAPL+IBM+SNE low hight
getyahoodb 01/01/2011 01/01/2012 GOOG+YHOO+AMZN open close volume
```



# Compilation #
```
Linux users:		gcc -o getyahoodb getyahoodb.c
Windows users:		gcc -D_WINDOWS_ -o getyahoodb getyahoodb.c
```

If you do not want to compile the program, executable files are also available for [download](http://code.google.com/p/get-yahoo-db/downloads/list).


# Installation #

Copy the executable `'getyahoodb'` to the system directory _/usr/bin_ for Linux
users, or _C:\windows\system32_ for Windows.


# Known problems #

If the output file comes a legend like "you're having trouble locating a
destination on Yahoo!," then one of the symbols is incorrect, or there is no
data available for selected date range, so it is necessary to change these
fields.



# Contact #

https://launchpad.net/getyahoodb, https://code.google.com/p/get-yahoo-db
