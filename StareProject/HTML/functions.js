function compareDecode(json) {
    var response = JSON.parse(json);
    
    var outputHTML = '<html><head><title>STARE</title><link rel="stylesheet" type="text/css" href="style.css" /></head><div id="wrapper"><h1>STARE</h1><p>This is STARE. A plagiarism detector.</p><p>Your document was plagiarized: <b><span id="certainty"></span>%</b></p><div id="plagiarism"><!-- This is where javascript inserts compare --></div></div><script src="http://crypto-js.googlecode.com/svn/tags/3.1.2/build/rollups/sha256.js"></script><script src="http://ajax.googleapis.com/ajax/libs/jquery/2.1.1/jquery.min.js"></script><script src="functions.js"></script></html>';
    document.open();
    document.write(outputHTML);
    document.close();
    //add the overall certainty
    document.getElementById('certainty').innerHTML = response.overallCertainty;
    //Start body of plagarized document
	for (i=0; i < response.ranking.length; ++i){
	    var plagiarism = document.createElement('div');
	    plagiarism.setAttribute("id", "section");
	    var inside = "";
	    //title area
	    inside += '<div id="doc-wrapper"><div id="left-column"><h3>' +
	        window.userDocTitle + '</h3></div>';
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
        if(json.hasOwnProperty("ranking")){
            compareDecode(JSON.stringify(json));
            return;
        }
        //window.location.href = 'output.html'; 
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

// getElementById
function $id(id) {
	return document.getElementById(id);
}
//output info
function Output(msg) {
	var m = $id("messages");
	m.innerHTML = msg;
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




