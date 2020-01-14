from django.shortcuts import render
import json
from django.db import connection
from django.http import HttpResponse
 
# 定义功能
def operationSql(infoType, Name):
    List = []

    if infoType == 1:
        tableName = 'area_day'
    elif infoType == 2:
        tableName = 'area_month'
    elif infoType == 3:
        tableName = 'line_day'
    elif infoType == 4:
        tableName = 'line_month'
    else:
        return 'infoType错误'

    cursor = connection.cursor() # 获取游标对象
    command = 'SELECT * FROM %s WHERE name="%s";' % (tableName, Name)
    cursor.execute(command)
    result = cursor.fetchall()  # 获取所有的数据

    for row in result:
        Dict = {}
        Dict['Name'] = row[1]
        Dict['lineLoss'] = row[2]
        if infoType == 1 or infoType == 3:
            Dict['Date'] = row[5].strftime("%Y-%m-%d")
        else:
            Dict['Date'] = row[5]
        List.append(Dict)

    return json.dumps(List, ensure_ascii=False)
 
# 接口函数
def post(request):
    if request.method == 'POST':  # 当提交表单时
        # 判断是否传参
        if request.POST:
            infoType = request.POST.get('infoType', 0)
            Name = request.POST.get('Name', '')
            if infoType != 0 and Name != '':
                result = operationSql(int(infoType), Name)
                return HttpResponse(result)
            else:
                return HttpResponse('输入错误')
        else:
            return HttpResponse('输入为空')
    else:
        return HttpResponse('方法错误')
