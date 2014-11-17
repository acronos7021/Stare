//Compare functions
function compareDecode(json) {
    var response = JSON.parse(json);
    
    var outputHTML = '<p>Your document was plagiarized: <b><span id="certainty"></span>%</b></p><div id="plagiarism"><!-- This is where javascript inserts compare --></div>';
    document.getElementById('wrapper').innerHTML = outputHTML;
    
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
	        '<span>This section was plagiarized: <br><b>' + Math.round(response.ranking[
	            i].certainty*100) + '%</b></span></div>  ' + response.ranking[
	            i].origSnip[2] + '</div></div>';
	    inside += '<div id="right-column"><div id=doc-box>' +
	        response.ranking[i].dataBaseSnip[0] +
	        '  <div id="popup" style="background-color: yellow;">' +
	        response.ranking[i].dataBaseSnip[1] +
	        '<span>This section was plagiarized: <br><b>' + Math.round(response.ranking[
	            i].certainty*100) + '%</b></span></div>  ' + response.ranking[
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

function compareSubmitButtonPress(){
    var id = generateID();
    document.cookie="id="+id;
    sendCompare(id, window.contents);
}

//end compare functions

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
    response=response.trim();
    var json = JSON.parse(response);
    if(json.command=="compare"){
        if(json.hasOwnProperty("ranking")){
            compareDecode(JSON.stringify(json));
            return;
        }
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
    else if(json.command=="getStyles"){
		fillStyles(json);
    }
    else if(json.command=="create"){
		fillCreate(json);
    }
    else if(json.command=="learn"){
		getBackLearn(json);
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


//
// initialize
function InitUpload() {

    var fileselect = $id("fileselect"),
        uploadbox = $id("uploadbox"),
        submitbutton = $id("submitbutton");

    // file select
    fileselect.addEventListener("change", FileSelectHandler, false);

    // file drop
    uploadbox.addEventListener("dragover", FileDragHover, false);
    uploadbox.addEventListener("dragleave", FileDragHover, false);
    uploadbox.addEventListener("drop", FileSelectHandler, false);
    uploadbox.style.display = "block";
}

// file drag hover
function FileDragHover(e) {
    e.stopPropagation();
    e.preventDefault();
    e.target.className = (e.type == "dragover" ? "hover" : "");
    saveE=e;
}
// file selection
function FileSelectHandler(e) {

    // cancel event and hover styling
    FileDragHover(e);

    // fetch FileList object
    var files = e.target.files || e.dataTransfer.files;

    // process last File object
    f = files[files.length-1];
    ParseFile(f);
    var r = new FileReader();
    r.onload = function(e) {
        contents = e.target.result;
    }
    r.readAsText(f);
}


function fillStyles(json){
	var select = document.getElementById("styleSelect");
	for (i=0; i < json.styles.length; ++i){
		var option = document.createElement('option');
		option.text = json.styles[i];
		select.add(option, 0);
	}
	
}

//TODO no comma after getStyles"
function getStyles(){
	var styles = {
		"command": "getStyles",
	}
	sendMessage(styles);
}

function createSubmitButtonPress(){
    var id = generateID();
    document.cookie="id="+id;
    var select = document.getElementById("styleSelect");
    var selectedText = select.options[select.selectedIndex].text;
	var create = {
		"command": "create",
		"clientID": id,
		"style": selectedText,
		"numberOfSentences": document.getElementById('numOfSentences').value
	}
	sendMessage(create);
}

function fillCreate(json){
	var select = document.getElementById("styleSelect");
    var selectedText = select.options[select.selectedIndex].text;
	var outputHTML = '<h2>Style of '+ selectedText +'</h2><p class="lead" style="width:60%; margin: 0 auto;">'+ json.document +'</p>';
    document.getElementById('wrapper').innerHTML = outputHTML;
}


function getBackLearn(json){
	if (json.result== "Success"){
		toastr.options = {
		  "positionClass": "toast-top-center"
		}
        toastr.success(document.getElementById('title').value+' uploaded successfully!', 'STARE Says');
	}
	
}

function learn(){
	sendLearn(window.contents, document.getElementById('author').value, document.getElementById('title').value, document.getElementById('publishYear').value);
}

function sendLearn(text, author, title, publishDate) {
	var learn = {
		"command": "learn",
		"documentText": text,
		"author": author,
		"title": title,
		"publishDate": publishDate
	}
	
    sendMessage(learn)
}

function ParseFile(file) {
    window.userDocTitle=file.name;
    	// display text
    if (file.type.indexOf("text") == 0) {
        var reader = new FileReader();
        reader.onload = function(e) {
            Output(
                "<p>Document information: <strong>" + file.name +
                "</strong> type: <strong>" + file.type +
                "</strong> size: <strong>" + Math.round(file.size/1024) +
                "</strong> Kilobytes</p>"+
                "<p><strong>Preview Uploaded Document:</strong></p><pre>" +
                e.target.result.replace(/</g, "&lt;").replace(/>/g, "&gt;") +
                "</pre>"+"<button type='button' class='btn btn-default' id='submitbutton' onclick='compareSubmitButtonPress();'>Submit</button>"
            );
        }
        reader.readAsText(file);
    }
}   
function InitLearn() {

    var fileselect = $id("fileselect"),
        uploadbox = $id("uploadbox"),
        submitbutton = $id("submitbutton");

    // file select
    fileselect.addEventListener("change", learnFileSelectHandler, false);

    // file drop
    uploadbox.addEventListener("dragover", FileDragHover, false);
    uploadbox.addEventListener("dragleave", FileDragHover, false);
    uploadbox.addEventListener("drop", learnFileSelectHandler, false);
    uploadbox.style.display = "block";
}

function learnParseFile(file) {
    window.userDocTitle=file.name;
    	// display text
    if (file.type.indexOf("text") == 0) {
        var reader = new FileReader();
        reader.onload = function(e) {
            Output(
                "<p>Document information: <strong>" + file.name +
                "</strong> type: <strong>" + file.type +
                "</strong> size: <strong>" + Math.round(file.size/1024) +
                "</strong> Kilobytes</p>"+
                "<p><strong>Preview Uploaded Document:</strong></p><pre>" +
                e.target.result.replace(/</g, "&lt;").replace(/>/g, "&gt;") +
                "</pre>"
            );
        }
        reader.readAsText(file);
    }
} 

function learnFileSelectHandler(e) {

    // cancel event and hover styling
    FileDragHover(e);

    // fetch FileList object
    var files = e.target.files || e.dataTransfer.files;

    // process last File object
    f = files[files.length-1];
    learnParseFile(f);
    var r = new FileReader();
    r.onload = function(e) {
        contents = e.target.result;
    }
    r.readAsText(f);
} 
