import json

prologRecords=[]

with open('cardinfoB.json') as cardsFile:
    data = json.load(cardsFile)['data']
    for card in data:
        prologTemplate = f"""cardBook(uniqueData(name('{card['name']}'), id({card['id']})),
            type(cardType('{card['type']}'),race('{card['race']}')),
            priceData(archetype('{card['archetype']}'),price({card['card_prices'][0]['amazon_price']})),
            attributes(element('{card['attribute']}'),level({card['level']}),atk({card['atk']}),def({card['def']}))
            ).\n"""
        prologRecords.append(prologTemplate)

print(len(prologRecords))
with open('knowledgeBase.txt', 'w') as prologFile:
    for record in prologRecords:
        prologFile.write(record)
