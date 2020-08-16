# -*- coding: utf-8 -*-
import csv

"""
# Merge various .csv files
files = ['rodada-1.csv', 'rodada-2.csv']

final_headers = ['', 'atletas.nome', 'atletas.slug', 'atletas.apelido', 'atletas.foto', 'atletas.atleta_id', 'atletas.rodada_id', 'atletas.clube_id', 'atletas.posicao_id', 'atletas.status_id', 'atletas.pontos_num',
                 'atletas.preco_num', 'atletas.variacao_num', 'atletas.media_num', 'atletas.jogos_num', 'atletas.clube.id.full.name','FF','FS','G','PI','CA','FC','DS','FT','DD','GS','FD','GC','SG','A','I','CV','PP']
header = set()
header.add(tuple(h for h in final_headers))

merged_rows = set()

for f in files:
    
    with open(f, 'r') as csv_in:
        csvreader = csv.reader(csv_in, delimiter=',')
        headers = dict((h, i) for i, h in enumerate(next(csvreader)))
        #print(headers)
        for row in csvreader:
            #print(row[headers['']])
            merged_rows.add(tuple(row[headers[x]] for x in final_headers))
        

with open('dados_2020.csv', 'w') as csv_out:
    csvwriter = csv.writer(csv_out, delimiter=',')
    csvwriter.writerows(header)
    csvwriter.writerows(merged_rows)

"""

"""
r = csv.reader(open('dados_2020.csv'))  # Here your csv file
lines = list(r)
lines[0].append('DP')
for i in range(1, len(lines)):
    lines[i].append('0')
writer = csv.writer(open('dados_2020.csv', 'w'))
writer.writerows(lines)
"""

"""
r = csv.reader(open('dados_2020.csv'))
lines = list(r)
for i in range(1, len(lines)):
    if(lines[i][15] == 'Palmeiras'): lines[i][7] = 0 #Palmeiras
    elif(lines[i][15] == 'Santos'): lines[i][7] = 1 #Santos
    elif(lines[i][15] == 'São Paulo'): lines[i][7] = 2 #São Paulo
    elif(lines[i][15] == 'Corinthians'): lines[i][7] = 3 #Corinthians
    elif(lines[i][15] == 'Flamengo'): lines[i][7] = 4 #Flamengo
    elif(lines[i][15] == 'Athlético-PR'): lines[i][7] = 5 #Atlético PR
    elif(lines[i][15] == 'Ceará'): lines[i][7] = 6 #Ceará
    elif(lines[i][15] == 'Bahia'): lines[i][7] = 7 #Bahia
    elif(lines[i][15] == 'Internacional'): lines[i][7] = 8 #Internacional
    elif(lines[i][15] == 'Atlético-MG'): lines[i][7] = 9 #Atlético MG
    elif(lines[i][15] == 'Botafogo'): lines[i][7] = 10 #Botafogo
    elif(lines[i][15] == 'Grêmio'): lines[i][7] = 11 #Gremio
    elif(lines[i][15] == 'Fluminense'): lines[i][7] = 12 #Fluminense
    elif(lines[i][15] == 'Sport'): lines[i][7] = 13 #Sport
    elif(lines[i][15] == 'Vasco'): lines[i][7] = 14 #Vasco
    elif(lines[i][15] == 'Atlético-GO'): lines[i][7] = 15 #Atlético-GO
    elif(lines[i][15] == 'Coritiba'): lines[i][7] = 16 #Coritiba
    elif(lines[i][15] == 'Goiás'): lines[i][7] = 17 #Goiás
    elif(lines[i][15] == 'Fortaleza'): lines[i][7] = 18 #Fortaleza
    elif(lines[i][15] == 'Bragantino'): lines[i][7] = 19 #Bragantino

writer = csv.writer(open('dados_2020.csv', 'w'))
writer.writerows(lines)
"""

"""
# Sort .csv file
with open('dados_2020.csv') as csvfile:
    spamreader = csv.DictReader(csvfile, delimiter=",")
    sortedlist = sorted(spamreader, key=lambda row: (row['atletas.posicao_id'], int(row['atletas.clube_id']), int(row['atletas.atleta_id']), int(row['atletas.rodada_id'])), reverse=False)

final_headers_order = ['atletas.posicao_id', 'atletas.clube_id', 'atletas.atleta_id','atletas.rodada_id', 'atletas.nome', 'atletas.slug', 'atletas.apelido', 'atletas.foto', 'atletas.status_id', 'atletas.pontos_num',
                 'atletas.preco_num', 'atletas.variacao_num', 'atletas.media_num', 'atletas.clube.id.full.name', 'FS', 'DS', 'PI', 'FC', 'G', 'FF', 'FT', 'FD', 'DD', 'GS', 'SG', 'A', 'CA', 'I', 'CV', 'PP', 'GC', 'DP', '', 'atletas.jogos_num']

with open('dados_2020.csv', 'w') as f:
    writer = csv.DictWriter(f, fieldnames=final_headers_order)
    writer.writeheader()
    for row in sortedlist:
        writer.writerow(row)
"""

"""
r = csv.reader(open('dados_2020.csv'))
lines = list(r)
for i in range(1, len(lines)):
    if(lines[i][14] == 'NA' or lines[i][14] == ''): lines[i][14] = '0'
    if(lines[i][15] == 'NA' or lines[i][15] == ''): lines[i][15] = '0'
    if(lines[i][16] == 'NA' or lines[i][16] == ''): lines[i][16] = '0'
    if(lines[i][17] == 'NA' or lines[i][17] == ''): lines[i][17] = '0'
    if(lines[i][18] == 'NA' or lines[i][18] == ''): lines[i][18] = '0'
    if(lines[i][19] == 'NA' or lines[i][19] == ''): lines[i][19] = '0'
    if(lines[i][20] == 'NA' or lines[i][20] == ''): lines[i][20] = '0'
    if(lines[i][21] == 'NA' or lines[i][21] == ''): lines[i][21] = '0'
    if(lines[i][22] == 'NA' or lines[i][22] == ''): lines[i][22] = '0'
    if(lines[i][23] == 'NA' or lines[i][23] == ''): lines[i][23] = '0'
    if(lines[i][24] == 'NA' or lines[i][24] == ''): lines[i][24] = '0'
    if(lines[i][25] == 'NA' or lines[i][25] == ''): lines[i][25] = '0'
    if(lines[i][26] == 'NA' or lines[i][26] == ''): lines[i][26] = '0'
    if(lines[i][27] == 'NA' or lines[i][27] == ''): lines[i][27] = '0'
    if(lines[i][28] == 'NA' or lines[i][28] == ''): lines[i][28] = '0'
    if(lines[i][29] == 'NA' or lines[i][29] == ''): lines[i][29] = '0'
    if(lines[i][30] == 'NA' or lines[i][30] == ''): lines[i][30] = '0'
    if(lines[i][31] == 'NA' or lines[i][31] == ''): lines[i][31] = '0'
writer = csv.writer(open('dados_2020.csv', 'w'))
writer.writerows(lines)
"""

"""
#REMOVER TODAS AS TUPLAS COM STATUS DIFERENTE DE PROVAVEL NESSA P#:
r = csv.reader(open('dados_2020.csv'))
lines = list(r)
#nao adiciona jogadores com status NULO
lines2 = [l for l in lines if(l[8][0] != 'N')]

writer = csv.writer(open('dados_2020.csv', 'w'))
writer.writerows(lines2)
"""

