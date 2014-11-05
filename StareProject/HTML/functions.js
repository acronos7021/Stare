function compareDecode(json) {
    var response = JSON.parse(json);

    //add the overall certainty
    document.getElementById('certainty').innerHTML = response.overallCertainty;

    //Start body of plagarized document
	for (i=0; i < response.ranking.length; ++i){
	    var plagiarism = document.createElement('div');
	    plagiarism.setAttribute("id", "section");
	    var inside = "";
	    //title section
	    inside += '<div id="doc-wrapper"><div id="left-column"><h3>' +
	        userDocTitle + '</h3></div>';
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
	    plagiarism.innerHTML = inside;
	    document.getElementById('plagiarism').appendChild(plagiarism);
	}
}

//This is for testing
var userDocTitle = "My Title";
compareDecode(
    '{"clientID":"ID","command":"compare","status":"#of characters process","documentText":"the document text here","overallCertainty":"50","ranking":[{"origSnip":["Aenean suscipit a metus eu aliquet.","Vestibulum lacus lorem, viverra sit amet tincidunt a, pharetra vitae mi.","Aliquam egestas rutrum tincidunt."],"dataBaseSnip":["Lorem ipsum dolor sit amet, consectetur adipiscing elit.","In molestie congue libero ut feugiat.","Etiam convallis arcu sit amet elit ullamcorper, rhoncus sodales lorem consequat."],"certainty":"10","documentTitle":"someTitle","foundDocumentID":"SomeDocID","foundSentenceID":"someSentID"},{"origSnip":["sentence1","sentence2","sentence3"],"dataBaseSnip":["sentence1","sentence2","sentence3"],"certainty":"100","documentTitle":"2nd Document","foundDocumentID":"SomeDocID","foundSentenceID":"someSentID"}]}'
);



function sendCompare(clientID, text) {
	//Still need to figure out clientID generation
	var compare = {
		"clientID": clientID,
		"command": "compare",
		"documentText": text
	}
	
    //do something to send it off here
    alert(sendMessage(compare));
}

function sendMessage(string){
	var xmlhttp=new XMLHttpRequest();
	xmlhttp.open("POST","some.php",true);
	xmlhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
	xmlhttp.send(string);
	return xmlhttp.responseText;
}


sendCompare("TheID", "The document text will go here");







