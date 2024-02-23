
Library used to generate reports in various formats and send to standard out or user's email.

Format      Description
----------- --------------------------------------------------------------------
CSV         Comma Separated Values with one header line of column names
EXCEL       Excel spreadsheet (XLSX format) with one header row of column names
TEXT        Report in text format. Paged.
PDF_VIEW    PDF for viewing on website. Paged.
PDF_EMAIL   PDF to send to user's email. Paged.
HTML        HTML format for disply on website. One header line.
EMAIL       HTML format to send to user's email. One header line.
APPEND_W    Invest system. Append to watchlist table. No header line.
REPLACE_W   Invest system. Replace rows in watchlist table. No header line.
TEMP        Invest system. Replace rows in temp table. No header line.

Reports support one level of breaks and subtotals, as well as grand totals.

Here is a typical usage statement for a report which uses this library:
```
    USAGE: rpt_portfolio -member id [options]
    options:
     -style {base*|mstar|full|fundamental|alerts|overnight|today|yoy}
     -type {all*|stock|etf|fixed|cash}
     -broker broker/account
     -sort {ticker*|gain|roi|compared|}
     -fmt {csv*|txt|html|email|xlsx|others not available
     -d   = debug, prints alert or report, does not send email
     * default value
```
Typical work flow in C program:
```
main ()
{
    /*----------------------------------------------------------
        get command line arguments
    ----------------------------------------------------------*/
    getargs ()

    /*----------------------------------------------------------
        open the database
    ----------------------------------------------------------*/
    initDB ()

    /*----------------------------------------------------------
        get data, writes to pipe delimited file.
    ----------------------------------------------------------*/
    getdata ()

    /*----------------------------------------------------------
        reads pipe delimited file. outputs to selected format.
    ----------------------------------------------------------*/
    dorpt ()

    /*----------------------------------------------------------
        send to screen or email.
    ----------------------------------------------------------*/
    rptdeliv ()

}
```

