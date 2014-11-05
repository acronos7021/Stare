function compareDecode(json) {
    var response = JSON.parse(json);
    //alert(response.clientID);
    //alert(response.ranking[0].origSnip[0]);
    var div = document.createElement('div');
    div.setAttribute("id", "section");
    var inside="";
    //title section
    inside += '<div id="doc-wrapper"><div id="left-column"><h3>'+response.ranking[0].documentTitle+'</h3></div>';
    inside += '<div id="right-column"><h3>'+response.ranking[0].documentTitle+'</h3></div></div>';
    
    //content section
    inside += '<div id="doc-wrapper"><div id="left-column"><div id=doc-box>'+ response.ranking[0].origSnip[0] +'</div></div>';
    inside += '<div id="right-column"><div id=doc-box>'+ response.ranking[0].dataBaseSnip[0] +'</div></div></div>';
    div.innerHTML=inside;
    document.getElementById('plag').appendChild(div);
    
}




compareDecode('{"clientID":"ID","command":"compare","status":"#of characters process","documentText":"the document text here","ranking":[{"origSnip":["sentence1","sentence2","sentence3"],"dataBaseSnip":["sentence1","sentence2","sentence3"],"certainty":"someValue","documentTitle":"someTitle","foundDocumentID":"SomeDocID","foundSentenceID":"someSentID"},{"origSnip":["sentence1","sentence2","sentence3"],"dataBaseSnip":["sentence1","sentence2","sentence3"],"certainty":"someValue","documentTitle":"someTitle","foundDocumentID":"SomeDocID","foundSentenceID":"someSentID"}]}');



function sendCompare(clientID, text){
    //do something to send it off here
}