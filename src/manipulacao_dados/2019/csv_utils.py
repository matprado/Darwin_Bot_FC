import csv


# inicio-0.csv change to 1 in atletas.rodada_id
r = csv.reader(open('inicio-0.csv'))  # Here your csv file
lines = list(r)
for i in range(1, len(lines)):
    lines[i][6] = '0'
writer = csv.writer(open('inicio-0.csv', 'w'))
writer.writerows(lines)


# Merge various .csv files
files = ['inicio-0.csv', 'rodada-1.csv', 'rodada-2.csv', 'rodada-3.csv', 'rodada-4.csv', 'rodada-5.csv', 'rodada-6.csv', 'rodada-7.csv', 'rodada-8.csv', 'rodada-9.csv', 'rodada-10.csv', 'rodada-11.csv', 'rodada-12.csv', 'rodada-13.csv', 'rodada-14.csv', 'rodada-15.csv', 'rodada-16.csv', 'rodada-17.csv', 'rodada-18.csv', 'rodada-19.csv',
         'rodada-20.csv', 'rodada-21.csv', 'rodada-22.csv', 'rodada-23.csv', 'rodada-24.csv', 'rodada-25.csv', 'rodada-26.csv', 'rodada-27.csv', 'rodada-28.csv', 'rodada-29.csv', 'rodada-30.csv', 'rodada-31.csv', 'rodada-32.csv', 'rodada-33.csv', 'rodada-34.csv', 'rodada-35.csv', 'rodada-36.csv', 'rodada-37.csv', 'rodada-38.csv']

ini_headers = ['', 'atletas.nome', 'atletas.slug', 'atletas.apelido', 'atletas.foto', 'atletas.atleta_id', 'atletas.rodada_id', 'atletas.clube_id', 'atletas.posicao_id', 'atletas.status_id', 'atletas.pontos_num',
               'atletas.preco_num', 'atletas.variacao_num', 'atletas.media_num', 'atletas.clube.id.full.name']

final_headers = ['', 'atletas.nome', 'atletas.slug', 'atletas.apelido', 'atletas.foto', 'atletas.atleta_id', 'atletas.rodada_id', 'atletas.clube_id', 'atletas.posicao_id', 'atletas.status_id', 'atletas.pontos_num',
                 'atletas.preco_num', 'atletas.variacao_num', 'atletas.media_num', 'atletas.clube.id.full.name', 'FS', 'RB', 'PE', 'FC', 'G', 'FF', 'FT', 'FD', 'DD', 'GS', 'SG', 'A', 'CA', 'I', 'CV', 'PP', 'GC', 'DP']
header = set()
header.add(tuple(h for h in final_headers))

ini = True

merged_rows = set()
for f in files:
    with open(f, 'r') as csv_in:
        csvreader = csv.reader(csv_in, delimiter=',')
        headers = dict((h, i) for i, h in enumerate(next(csvreader)))
        for row in csvreader:
            if(ini):
                merged_rows.add(tuple(row[headers[x]] for x in ini_headers))
            else:
                merged_rows.add(tuple(row[headers[x]] for x in final_headers))
        ini = False


with open('dados_2019.csv', 'w') as csv_out:
    csvwriter = csv.writer(csv_out, delimiter=',')
    csvwriter.writerows(header)
    csvwriter.writerows(merged_rows)


# Sort .csv file
with open('dados_2019.csv', newline='') as csvfile:
    spamreader = csv.DictReader(csvfile, delimiter=",")
    sortedlist = sorted(spamreader, key=lambda row: (row['atletas.posicao_id'], int(row['atletas.clube_id']), int(row['atletas.atleta_id']), int(row['atletas.rodada_id'])), reverse=False)

final_headers_order = ['atletas.posicao_id', 'atletas.clube_id', 'atletas.atleta_id','atletas.rodada_id', 'atletas.nome', 'atletas.slug', 'atletas.apelido', 'atletas.foto', 'atletas.status_id', 'atletas.pontos_num',
                 'atletas.preco_num', 'atletas.variacao_num', 'atletas.media_num', 'atletas.clube.id.full.name', 'FS', 'RB', 'PE', 'FC', 'G', 'FF', 'FT', 'FD', 'DD', 'GS', 'SG', 'A', 'CA', 'I', 'CV', 'PP', 'GC', 'DP', '']

with open('dados_2019.csv', 'w') as f:
    writer = csv.DictWriter(f, fieldnames=final_headers_order)
    writer.writeheader()
    for row in sortedlist:
        writer.writerow(row)
