function compareDecode(json) {
    var response = JSON.parse(json);

    //add the overall certainty
    document.getElementById('certainty').innerHTML = response.overallCertainty;

    //Start body of plagarized document
	for (i=0; i < response.ranking.length; ++i){
	    var div = document.createElement('div');
	    div.setAttribute("id", "section");
	    var inside = "";
	    //title section
	    inside += '<div id="doc-wrapper"><div id="left-column"><h3>' +
	        response.ranking[i].documentTitle + '</h3></div>';
	    inside += '<div id="right-column"><h3>' + response.ranking[i]
	        .documentTitle + '</h3></div></div>';
	
	    //content section
	    inside +=
	        '<div id="doc-wrapper"><div id="left-column"><div id=doc-box>' +
	        response.ranking[i].origSnip[0] +
	        '  <div id="popup" style="background-color: yellow;">' +
	        response.ranking[i].origSnip[1] +
	        '<span>This section was plagiarized: <b>' + response.ranking[
	            i].certainty + '%</b></span></div>  ' + response.ranking[
	            i].origSnip[2] + '</div></div>';
	    inside += '<div id="right-column"><div id=doc-box>' +
	        response.ranking[i].dataBaseSnip[0] +
	        '  <div id="popup" style="background-color: yellow;">' +
	        response.ranking[i].dataBaseSnip[1] +
	        '<span>This section was plagiarized: <b>' + response.ranking[
	            i].certainty + '%</b></span></div>  ' + response.ranking[
	            i].dataBaseSnip[2] + '</div></div></div>';
	    div.innerHTML = inside;
	    document.getElementById('plag').appendChild(div);
	}
}




compareDecode(
    '{"clientID":"ID","command":"compare","status":"#of characters process","documentText":"the document text here","overallCertainty":"50","ranking":[{"origSnip":["Aenean suscipit a metus eu aliquet.","Vestibulum lacus lorem, viverra sit amet tincidunt a, pharetra vitae mi.","Aliquam egestas rutrum tincidunt."],"dataBaseSnip":["Lorem ipsum dolor sit amet, consectetur adipiscing elit.","In molestie congue libero ut feugiat.","Etiam convallis arcu sit amet elit ullamcorper, rhoncus sodales lorem consequat."],"certainty":"someValue","documentTitle":"someTitle","foundDocumentID":"SomeDocID","foundSentenceID":"someSentID"},{"origSnip":["sentence1","sentence2","sentence3"],"dataBaseSnip":["sentence1","sentence2","sentence3"],"certainty":"someValue","documentTitle":"2nd Document","foundDocumentID":"SomeDocID","foundSentenceID":"someSentID"}]}'
);



function sendCompare(clientID, text) {
    //do something to send it off here
}
