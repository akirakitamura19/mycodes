
#!/usr/bin/python
# coding: utf-8
# 日経平均資料室(http://indexes.nikkei.co.jp/nkave/archives/data)の日次データの差分をクロールして追加するプログラム

import urllib2
import lxml.html
import csv
import re
import time
from datetime import datetime as dt
from datetime import timedelta


def main():
    filename = "nikkei225.csv"
    f = open(filename, 'a+')
    csvwriter = csv.writer(f)

    # ファイルが存在しない（または空）の場合はこのfrom_dateで指定された日時を起点に取得
    from_date = "1949/05/16"

    today = dt.today()

    lastline = tail(f, 1)
    if lastline == "":
        lastdate_dt = dt.strptime(from_date, "%Y/%m/%d") - timedelta(days=1)
        lastdate_year = lastdate_dt.year
        lastdate_month = lastdate_dt.month
    else:
        lastdate = lastline.split(",")[0]
        lastdate_dt = dt.strptime(lastdate, "%Y/%m/%d")
        lastdate_year = lastdate_dt.year
        lastdate_month = lastdate_dt.month
        # ファイル最終行を読んでファイルの最終更新日を取得

    # URL
    url_1 = "http://indexes.nikkei.co.jp/nkave/statistics/dataload?type=daily&year="
    url_2 = "&month="

    # FF9.0のUA
    user_agent = "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:9.0.1) Gecko/20100101 Firefox/9.0.1"

    for i in range(lastdate_year, today.year+1):
        for j in range(1, 12+1):
            if not(i == lastdate_year and j < lastdate_month):

                url = url_1 + str(i) + url_2 + str(j)  # 呼び出すurl

                req = urllib2.Request(url, None, {'User-Agent': user_agent})
                res = urllib2.urlopen(req)
                src = re.sub(r"<!--daily_changing-->", "", res.read())  # 取得html内のコメントアウトを消去

                html = lxml.html.fromstring(src)
                table = html.cssselect("table")[1][2:]

                for tr in table:
                    tr_list = []
                    for (k, td) in enumerate(tr):
                        value = lxml.html.tostring(td, method="text", encoding="utf-8").strip()
                        if k == 0:  # 一番左の列は日付なのでそのまま
                            value_modified = value
                        elif value == "-":  # 1985/03/23以前は終値以外が"-"になっているので処理
                            value_modified = float("nan")
                        else:
                            value_modified = value.replace(",", "")  # 数値からカンマ区切りを消す
                        tr_list.append(value_modified)

                    latestdate_dt = dt.strptime(tr_list[0], "%Y/%m/%d")
                    if latestdate_dt > lastdate_dt:
                        csvwriter.writerow(tr_list)  # ファイル最終行より新しければ書き込み

                time.sleep(1)  # サーバーへの思いやり
    f.close()


def tail(f, lines=20):
    total_lines_wanted = lines

    BLOCK_SIZE = 1024
    f.seek(0, 2)
    block_end_byte = f.tell()
    lines_to_go = total_lines_wanted
    block_number = -1
    blocks = []  # blocks of size BLOCK_SIZE, in reverse order starting
                 # from the end of the file
    while lines_to_go > 0 and block_end_byte > 0:
        if (block_end_byte - BLOCK_SIZE > 0):
            # read the last block we haven't yet read
            f.seek(block_number*BLOCK_SIZE, 2)
            blocks.append(f.read(BLOCK_SIZE))
        else:
            # file too small, start from begining
            f.seek(0, 0)
            # only read what was not read
            blocks.append(f.read(block_end_byte))
        lines_found = blocks[-1].count('\n')
        lines_to_go -= lines_found
        block_end_byte -= BLOCK_SIZE
        block_number -= 1
    all_read_text = ''.join(reversed(blocks))
    return '\n'.join(all_read_text.splitlines()[-total_lines_wanted:])
# このtail関数はStackOverflowから拝借
# http://stackoverflow.com/questions/136168/get-last-n-lines-of-a-file-with-python-similar-to-tail

if __name__ == '__main__':
    main()
