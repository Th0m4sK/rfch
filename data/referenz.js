


function myFunction() {
  var x = document.getElementById("myLinks");
  if (x.style.display === "block") {
    x.style.display = "none";
  } else {
    x.style.display = "block";
  }
}
 
 

 function saveRef() {
   console.log("SaveRef",document.getElementById("RefP").value);
   let sendtext = ["RefP",document.getElementById("RefP").value];
   

   post( sendtext);
   document.getElementById("RefP").style.border = "20px solid #22ca2a";
   
}

function fkt_RefP() {
  console.log("input");
  document.getElementById("RefP").style.border = "20px solid #ff0202";
}

function post(text) {
  var i;
  let sendtext = "";
  for (i = 0; i < text.length; i++) {
    
    sendtext += text[i]+"/";
  }
 
     
  console.log(sendtext);
  let xhr = new XMLHttpRequest();
  xhr.open("POST", "/data", true);
  xhr.setRequestHeader("Content-Type", "text/plain");
  xhr.send(sendtext);
}

function fkt_load() {
  let xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      const DATA = this.responseText.split(";");
        document.getElementById("RefP").value = parseFloat(DATA[0]);
       }
  };
  xhttp.open("GET", "/RefP", true);
  xhttp.send();
}


window.addEventListener("load", fkt_load);
document.getElementById("RefP").addEventListener("input", fkt_RefP);

 