const read1 = true;
let maxBereich = 0;
let nM = 0;
let LastnM = 0;


function myFunction() {
  var x = document.getElementById("myLinks");
  if (x.style.display === "block") {
    x.style.display = "none";
  } else {
    x.style.display = "block";
  }
}

function getValue(Name,mul) {
  return document.getElementById(Name).value
}

function Ein1() {
  let sendtext = ["EIN"];
  sendtext.push("60")
  post(sendtext);
}
function Ein2() {
  let sendtext = ["EIN"];
  sendtext.push("120")
  post(sendtext);
}
function Ein3() {
  let sendtext = ["EIN"];
  sendtext.push("240")
  post(sendtext);
}
function Ein4() {
  let sendtext = ["EIN"];
  sendtext.push("360")
  post(sendtext);
}
function AUS() {
  let sendtext = ["EIN"];
  sendtext.push("0")
  post(sendtext);
}

function save() {
  console.log("SavePar");
  let sendtext = ["savM"];
  var i;
  for (i = 1; i < 7; i++){
      sendtext.push(getValue("f"+i.toString(),1))
}
 post(sendtext);

  document.getElementById("SAVEPAR").style.background = "#22ca2a";
  
}

function fkt_load() {
                  let xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                  
                  if (this.readyState == 4 && this.status == 200) {
                    const DATA = this.responseText.split("/");
                    console.log(DATA)
                    let z = 0;
                    document.getElementById("f1").value= DATA[z];
                    z += 1;
                    document.getElementById("f2").value = DATA[z];
                    z += 1;
                    document.getElementById("f3").value = DATA[z];
                    z += 1;
                    document.getElementById("f4").value = DATA[z];
                    z += 1;
                    document.getElementById("f5").value=  DATA[z];
                    z += 1;
                    document.getElementById("f6").value= parseInt(DATA[z]);
                   
                  }
                  

                };
                xhttp.open("GET", "/GetData", true);
                xhttp.send();
              
              }
    

    

 function post(text) {
  var i;
  let sendtext = "";
  for (i = 0; i < text.length; i++) {
    
    sendtext += text[i]+"/";
  }
 
     
  console.log(sendtext);
  let xhr = new XMLHttpRequest();
  xhr.open("POST", "/SendData", true);
  xhr.setRequestHeader("Content-Type", "text/plain");
  xhr.send(sendtext);
}




window.addEventListener("load", fkt_load);






