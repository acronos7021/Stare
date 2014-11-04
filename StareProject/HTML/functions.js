function compareDecode(json) {
    var response = JSON.parse(json);
    alert(response.clientID);
    alert(response.ranking[0].origSnip[0]);
    
}




compareDecode('{"clientID":"ID","command":"compare","status":"#of characters process","documentText":"the document text here","ranking":[{"origSnip":["sentence1","sentence2","sentence3"],"dataBaseSnip":["sentence1","sentence2","sentence3"],"certainty":"someValue","sentenceID":"someValue","foundDocumentID":"SomeValue","foundSentenceID":"someValue"},{"origSnip":["sentence1","sentence2","sentence3"],"dataBaseSnip":["sentence1","sentence2","sentence3"],"certainty":"someValue","sentenceID":"someValue","foundDocumentID":"SomeValue","foundSentenceID":"someValue"}]}');