# Odds QT app

by Andrey Kolesnikov & Alexey Shvarev

### Links
- [QT Client repository](https://github.com/AlexFire-Dev/OddsClientQT "QT Client repository")
- [Server repository](https://github.com/AlexFire-Dev/OddsServer "Server repository")
- [API Tester](https://af.shvarev.com/api/v1/swagger/ "API Tester")

### Overview
 - Odds_helper is a decision-making assistant for people playing on esports betting. Providing an opportunity to sort bookmakers, leagues. Based on the percentage of the drop and on the fluctuation of the coefficient change graphs, users of our application can make their own forecasts.
 
### Functionality
- For our application, we use the official API for bookmaker parsing.
- We update our database every 15 minutes using asynchronous requests.
- We process the received data on our server.
- After authorization, we send a request to the server, after which we receive our database in json format.
- With the help of QT modules, we parse and form a selection based on filters in the application.
- By clicking on a line, you can draw a graph.