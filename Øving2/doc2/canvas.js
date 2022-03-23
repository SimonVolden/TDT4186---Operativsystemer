var boatColor = "brown"

function changeBoatColor(color){
    boatColor = color;
    draw_boat();
}

//canvas drawing follows
function draw_rektangle(x, y, width, length, color){
    let canvas = document.getElementById("myCanvas");
    let ctx = canvas.getContext("2d");
    ctx.fillStyle = color;
    ctx.fillRect(x, y, width, length);
}

function draw_sail(color){
    let canvas = document.getElementById("myCanvas");
    if (canvas.getContext) {
        let ctx = canvas.getContext('2d');
        ctx.globalAlpha = sailOpacity;
        ctx.beginPath();
        ctx.moveTo(260, 20);
        ctx.lineTo(500, 180, );
        ctx.lineTo(110,180);
        ctx.fillStyle = color;
        ctx.fill();
        ctx.globalAlpha = 1;
      }
}

function draw_boat_bottom(){
    var canvas = document.getElementById('myCanvas');
    if (canvas.getContext) {
        var ctx = canvas.getContext('2d');
        ctx.beginPath();
        ctx.moveTo(50, 210);
        ctx.quadraticCurveTo(200, 500, 550, 210);
        ctx.fillStyle = boatColor;
        ctx.fill();
    }
}

function draw_boat_name(name, color){
    var canvas = document.getElementById("myCanvas");
    var ctx = canvas.getContext("2d");
    ctx.font = "30px Verdana";
    ctx.fillStyle = color
    ctx.fillText(name, 100, 265);
}

function draw_sail_flag(color){
    var canvas = document.getElementById("myCanvas");
    var ctx = canvas.getContext("2d");
    
    ctx.beginPath();
    ctx.moveTo(260, 20);
    ctx.lineTo(200, 10);
    ctx.lineTo(260, 0);
    ctx.fillStyle = color;
    ctx.fill();
}
function changeSailOpacity(value) {
    //console.log("hello world");
    sailOpacity = value
    draw_boat()
}

function draw_hidden_message(message){
    var canvas = document.getElementById("myCanvas");
    var ctx = canvas.getContext("2d");
    ctx.font = "20px Verdana";
    ctx.fillStyle = "red"
    ctx.fillText(message, 260, 150);
}

function draw_boat() {
    //draw_flipped_triangle("brown")
    draw_rektangle(250,10,10,200,"black");
    draw_hidden_message("help me!");
    draw_sail("lightblue")
    draw_boat_bottom();
    draw_boat_name("Boaty McBoatface II", "white");
    draw_sail_flag("pink");

}

//window.requestAnimationFrame(draw_boat)
//https://stackoverflow.com/questions/3057162/moving-an-image-across-a-html-canvas
var imgTag = new Image();
var canvas = document.getElementById('myCanvas');
var ctx = canvas.getContext("2d");
var x = 0;
var y = 150;
var sailOpacity = 1

imgTag.onload = animate;
imgTag.src = "https://opengameart.org/sites/default/files/preview_742.png";   // load image

function animate() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);  // clear canvas
  draw_boat()
  ctx.drawImage(imgTag, x, y);                       // draw image at current position
  x -= 1;
  if (x > -314) {
    requestAnimationFrame(animate)
    }       // loop
  else {
    x=0
    requestAnimationFrame(animate)
  }
}

