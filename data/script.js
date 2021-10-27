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
  return document.getElementById(Name).value*mul
}
function setValue(Name,val) {
  document.getElementById(Name).value = val;
}


function setMem() {
  if (nM < 10 && document.getElementById("SOLL" + (nM).toString()).value > -1) {
    document.getElementById("NEXT").innerText = "Ⓝ M" + (nM+1).toString()
    document.getElementById("SOLL" + LastnM.toString()).style.border = "20px solid #22ca2a"
    LastnM =nM
    document.getElementById("SOLL" + (nM).toString()).style.border = "20px solid #ffc000"
    
  }
  else {
    document.getElementById("NEXT").innerText = "Ⓝ M1"
    document.getElementById("SOLL0").style.border = "20px solid #ffc000"
    document.getElementById("SOLL" + LastnM.toString()).style.border = "20px solid #22ca2a"
    LastnM=0
  }
}


setInterval(
              function () {
                  let xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function () {
                  
                  if (this.readyState == 4 && this.status == 200) {
                    const DATA = this.responseText.split("/");
                    console.log(DATA)
                    document.getElementById("IST").innerText= DATA[0];
                    if (DATA[1] === "0") {
                    document.getElementById("IST").style.border = "20px solid #22ca2a";
                    }
                    if (DATA[1] === "9") {
                      document.getElementById("IST").style.border = "20px solid #ff0000";
                      document.getElementById("IST").innerText= DATA[0]+"REF";
                    }
                    maxBereich = parseFloat(DATA[2]);
                    nM=parseInt(DATA[3])
                    setMem();
                  }
                  if (parseFloat(document.getElementById("IST").innerText) === parseFloat(document.getElementById("SOLL").value)) {
                    document.getElementById("SOLL").style.border = "20px solid #22ca2a";
                  }
                  else {
                    document.getElementById("SOLL").style.border = "20px solid #ff0000";
                  }

                };
                xhttp.open("GET", "/aktPos", true);
                xhttp.send();
              
              }
  , 1000);

  


function nextM() {
  let soll = -1
  let z = nM
  while (soll === -1 && z < 10) {
    if (z === 9) {
      nM = 0
      soll = document.getElementById("SOLL" + nM).value

    }
    else {
      z += 1;
      nM += 1;
    }
    if (document.getElementById("SOLL" + nM.toString()).value > -1)
      soll = document.getElementById("SOLL" + nM.toString()).value
      document.getElementById("SOLL").value=soll
  }
  post(["NextM", nM])
  if (nm === 0) {
    allGreen();
  }
  setMem();
 
}  

function checkArbeitsbereich(data) {
  let Alarm = false;
  let _ist = parseFloat(document.getElementById("IST").innerText)
  let msg = ""

  if (data[2] === "rel") {
    _ist = _ist + data[1]
  }
  if (data[2] === "abs") {
    _ist = data[1];
  }
    if (_ist  < 0){
      msg = "absolute Position < als 0mm";
    Alarm = true;
  }
  if (_ist > maxBereich){
  msg = "absolute Position > als "+maxBereich.toString()+"mm";
  Alarm = true;
}

  if (Alarm) {
    alert(msg);
    return false;
}
  return true;
}

function fkt_boarder(ev) {
  console.log("input");
  document.getElementById(ev.target.id).style.border = "20px solid #ff0202";
  document.getElementById("SAVEPAR").style.backgroundColor = "#ff0202";
  
}
function allGreen() {
  var i;
  for (i = 0; i < 10; i++){
    document.getElementById("SOLL"+i.toString()).style.border = "20px solid #22ca2a";
  }
 
}
function save() {
  console.log("SavePar");
  let sendtext = ["savM"];
  var i;
  for (i = 0; i < 10; i++){
      sendtext.push(getValue("SOLL"+i.toString(),1))
}
 post(sendtext);
  allGreen();
  document.getElementById("SAVEPAR").style.background = "#22ca2a";
  
}


function relMinus() {
    
    
  let sendtext = ["SOLL", 0, "rel"]
  
  sendtext[1] = document.getElementById("SOLL").value * -1
  if (checkArbeitsbereich(sendtext)) {
    post(sendtext);
}
  document.getElementById("IST").style.border = "20px solid #ffff00";
     
}
    function relPlus() {
    
    
      let sendtext = ["SOLL", 0, "rel"]
    
        sendtext[1] = document.getElementById("SOLL").value 
    
        if (checkArbeitsbereich(sendtext)) {
          post(sendtext);
      }
   document.getElementById("IST").style.border = "20px solid #ffff00";
   
 }

 function runMotor() {
   console.log("Run Motor", document.getElementById("SOLL").value);
   
   let sendtext = ["SOLL",0,"abs"]
 
  sendtext [1]= document.getElementById("SOLL").value
    
  if (checkArbeitsbereich(sendtext)) {
    post(sendtext);
}
  document.getElementById("IST").style.border = "20px solid #ffff00";
  
}



function stopMotor() {
  
  let sendtext = ["STOP",1];
  
 
    post(sendtext);



  
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
      const DATA = this.responseText.split("/");
      var i;
      for (i = 0; i < 10; i++){
        setValue("SOLL"+i.toString(), parseFloat(DATA[i]));
      }
      
      document.getElementById("IST").innerText= DATA[10];
      document.getElementById("SOLL").value= parseFloat (DATA[11]);
      allGreen();
       }
  };
  xhttp.open("GET", "/savM", true);
  xhttp.send();
}


window.addEventListener("load", fkt_load);

document.getElementById("SOLL0").addEventListener("input", fkt_boarder);
document.getElementById("SOLL1").addEventListener("input", fkt_boarder);
document.getElementById("SOLL2").addEventListener("input", fkt_boarder);
document.getElementById("SOLL3").addEventListener("input", fkt_boarder);
document.getElementById("SOLL4").addEventListener("input", fkt_boarder);
document.getElementById("SOLL5").addEventListener("input", fkt_boarder);
document.getElementById("SOLL6").addEventListener("input", fkt_boarder);
document.getElementById("SOLL7").addEventListener("input", fkt_boarder);
document.getElementById("SOLL8").addEventListener("input", fkt_boarder);
document.getElementById("SOLL9").addEventListener("input", fkt_boarder);





