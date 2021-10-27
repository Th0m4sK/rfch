


function myFunction() {
  var x = document.getElementById("myLinks");
  if (x.style.display === "block") {
    x.style.display = "none";
  } else {
    x.style.display = "block";
  }
}
 
 

 function saveSet() {
   console.log("SaveRef",document.getElementById("RefP").value);
   let sendtext = document.getElementById("RefP").value*10;
   console.log(sendtext);
   post("saveRef",sendtext)
   
}
function post(url, text) {
  let xhr = new XMLHttpRequest();
  xhr.open("POST", url, true);
  xhr.setRequestHeader("Content-Type", "text/plain");
  xhr.send(text);
 }

 