function compareDecode(json) {
    var response = JSON.parse(json);
    
    //TODO add most of HTML for output page.  removed from template due to progress bar.
    //TODO replace output HTML with what would be needed for update page instead.
    
    //add the overall certainty
    document.getElementById('certainty').innerHTML = response.overallCertainty;
    //Start body of plagarized document
	for (i=0; i < response.ranking.length; ++i){
	    var plagiarism = document.createElement('div');
	    plagiarism.setAttribute("id", "section");
	    var inside = "";
	    //title area
	    inside += '<div id="doc-wrapper"><div id="left-column"><h3>' +
	        userDocTitle + '</h3></div>';
	    inside += '<div id="right-column"><h3>' + response.ranking[i]
	        .documentTitle + '</h3></div></div>';
	    //content area
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

function sendCompare(clientID, text) {
	//Still need to figure out clientID generation
	var compare = {
		"clientID": clientID,
		"command": "compare",
		"documentText": text
	}
	
    //do something to send it off here
    sendMessage(compare);
    //window.location.href = 'output.html';
}

function checkCompare(id){
	var compare = {
		"clientID": clientID,
		"command": "checkCompare",
	}
	sendMessage(compare);
}

function sendMessage(string){
    var data=JSON.stringify(string);
	var xmlhttp=new XMLHttpRequest();
	xmlhttp.open("POST","phpAndCppTalk.php",true);
    xmlhttp.onreadystatechange=function()
    {
    if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {
        callback(xmlhttp.responseText);
        }
    }
	xmlhttp.setRequestHeader("Content-Type", "application/json");
	xmlhttp.send(data);
}

function callback(response){
    var json = JSON.parse(response);
    if(json.command=="compare"){
        //compareDecode(JSON.stringify(json));
        window.location.href = 'output.html'; 
    }
    else if(json.command=="checkCompare"){
        if(json.hasOwnProperty("ranking")){
            compareDecode(JSON.stringify(json));
            return;
        }
        //update the progress bar here
        setTimeout(check, 1000);  //wait a second before trying again
        checkCompare(getCookie("id"));
    }
}

function getCookie(cname) {
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for(var i=0; i<ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0)==' ') c = c.substring(1);
        if (c.indexOf(name) != -1) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}


function sendLearn(text, author, title, publishDate) {
	var learn = {
		"command": "learn",
		"documentText": text,
		"author": author,
		"title": title,
		"publishDate": publishDate
	}
	
    //do something to send it off here
    alert(sendMessage(learn));
}

function sendCreate(id, styleID, numOfSent) {
	var create = {
		"clientID": id,
		"command": "create",
		"styleID": styleID,
		"numberOfSentences": numOfSent
	}
	
    //do something to send it off here
    alert(sendMessage(create));
}

function generateID(){
	var d = new Date();
	var n=CryptoJS.lib.WordArray.random(128)+d.getTime();
	var id = CryptoJS.SHA256(n);
	return d.getTime();
}

function readSingleFile(evt) {
    //Retrieve the first (and only!) File from the FileList object
    var f = evt.target.files[0];
    var contents;
    if (f) {
        var r = new FileReader();
        r.onload = function(e) {
            contents = e.target.result;
            var id = generateID();
            document.cookie="id="+id;
            sendCompare(id, contents);
        }
        r.readAsText(f);
    } else {
        alert("Failed to load file");
    }
}
//sendCompare("TheID", "The document text will go here");
//This is for testing
var userDocTitle = "My Title"; // this will be set using a cookie in the future.
//compareDecode(
//    '{"clientID":"ID","command":"compare","status":"#of characters process","documentText":"the document text here","overallCertainty":"50","ranking":[{"origSnip":["Aenean suscipit a metus eu aliquet.","Vestibulum lacus lorem, viverra sit amet tincidunt a, pharetra vitae mi.","Aliquam egestas rutrum tincidunt."],"dataBaseSnip":["Lorem ipsum dolor sit amet, consectetur adipiscing elit.","In molestie congue libero ut feugiat.","Etiam convallis arcu sit amet elit ullamcorper, rhoncus sodales lorem consequat."],"certainty":"10","documentTitle":"someTitle","foundDocumentID":"SomeDocID","foundSentenceID":"someSentID"},{"origSnip":["sentence1","sentence2","sentence3"],"dataBaseSnip":["sentence1","sentence2","sentence3"],"certainty":"100","documentTitle":"2nd Document","foundDocumentID":"SomeDocID","foundSentenceID":"someSentID"}]}'
//);

//test generate id
//alert("Your ID is:\n"+generateID());
var compare = {"command":"compare","overallCertainty":"100","ranking":[{"origSnip":["sentence1","sentence2","sentence3"],"dataBaseSnip":["sentence1","sentence2","sentence3"],"certainty":"80","documentTitle":"someTitle","foundDocumentID":"SomeDocID","foundSentenceID":"someSentID"},{"origSnip":["sentence1","sentence2","sentence3"],"dataBaseSnip":["sentence1","sentence2","sentence3"],"certainty":"90","documentTitle":"someTitle","foundDocumentID":"SomeDocID","foundSentenceID":"someSentID"}]}
//sendMessage(compare);



