# WEB_service
### Результаты игры
### Описание
Данный веб-сервис предназначен для хранения и отображения результатов некоторой игры. Результаты отображаются в виде списка
1.	Имя1 Счет1
2.	Имя2 Счет2

Для удобства отображаются первые десять лучших результатов.
Пользователь может добавлять свой результат, указав свое имя и счет в игре. Также у пользователя есть возможность увидеть, на каком месте он находится в списке (если пользователь с одним и тем же именем добавил несколько результатов, они отобразятся в порядке невозрастания счета).
### API
1. Получить список 10 лучших результатов <br />
Request: GET /results <br />
Response: 	200 OK <br />
		Content-Type application/vnd.results.list+json <br />
```
		{
		 	“links”: {
				“self”: “/results”
			}, 
			“results”: [
				{
					“name”: “Ivan Ivanov”,
					“score”: 100,
					“links”: {
						“self”: “/results/Ivan Ivanov”
					}
				},
				{
					“name”: “Peter Pertov”,
					“score”: 90,
					“links”: {
						“self”: “/results/Peter Petrov”
					}
				},
				…
			]
		}
```
2. Получить результаты человека с данным именем. Кроме имени и счета возвращается позиция в общем списке <br />
Request: GET /results/{name} <br />
Response: 	200 OK <br />
		Content-Type application/vnd.results.player+json <br />
```
		{
			“links”: {
				“self”: “/results/Dasha Ivanova”
			},
			“player”: {
				"name": "Dasha Ivanova",
				"results": [{
					“number”: 93,
					“score”: 120
				},
				{
					“number”: 205,
					“score”: 85
				},
				…
				]
			}
		}

```
Response:	404 Not Found <br />
3. Добавить свой результат <br />
Request: 	POST /results <br />
		Content-Type: vnd.results.player+json
```
		{
			“links”: {
				“self”: “”
			},
			“player”: {
				"name": "Peter Ivanov",
				"results": [{
					“number”: ,
					“score”: 200
				}
				]
			}
		}

```
Response: 	201 Created <br />
			Location: /results/{name} <br />
Response: 400 Invalid Input

Используется REST <br />
1. Клиент-сервер. Реализовывается логика сервера. Клиенту нужно знать API. <br />
2. Отсутствите состояния. Мы не храним на сервере состояние клиента. <br />
3. Кеширование. Думаю, что не стоит его реализовывать, т.к. данные могут постоянно изменяться. При кешировании клиент может не получить достоверной информации. <br />
4. Единообразие интерфейса. Используются гипермедиа, сообщения "самоописываемые", клиент манипулирует данными через представление. <br />
5. Слои. Не знаю, будет ли использоваться промежуточный сервер.
