function draw_square(x, y, w, h) {
    var canvas = document.getElementById('canvas');
    if (canvas.getContext) {
        var ctx = canvas.getContext('2d');

        ctx.fillStyle = 'rgb(255, 195, 191)';
        ctx.fillRect(x, y, w, h);

    }
}

function draw_circles(startx, starty, size, colour) {
    var canvas = document.getElementById('canvas');
    var ctx = canvas.getContext('2d');

    ctx.beginPath();
    var x = startx + 25; // x coordinate
    var y = starty + 25; // y coordinate
    var radius = size; // Arc radius
    var startAngle = 0; // Starting point on circle
    var endAngle = Math.PI + (Math.PI * 2) / 2; // End point on circle
    ctx.arc(x, y, radius, startAngle, endAngle, false);
    ctx.fillStyle = colour;
    /* ctx.stroke(); */
    ctx.fill();


    
}

function draw_semi_circles(startx, starty, size, colour, angle) {
    var canvas = document.getElementById('canvas');
    if (canvas.getContext) {
        var ctx = canvas.getContext('2d');
        ctx.beginPath();
        var x = startx + 25; // x coordinate
        var y = starty + 25; // y coordinate
        var radius = size; // Arc radius
        var startAngle = angle; // Starting point on circle
        var endAngle = angle + Math.PI; // End point on circle
        ctx.arc(x, y, radius, startAngle, endAngle, false);
        ctx.fillStyle = colour;
        /* ctx.stroke(); */
        ctx.fill();


    }
    
}


/* https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Advanced_animations */
function draw() {
  var counter = 0;
  var canvas = document.getElementById("canvas");
  var ctx = canvas.getContext("2d");
  var raf;
  var running = false;

  var ball = {
    x: 260,
    y: 250,
    vx: -3,
    vy: -10,
    radius: 50,
    color: 'rgb(255, 145, 75)',
  };
  var ball2 = {
    x: 305,
    y: 445,
    vx: 4,
    vy: -10,
    radius: 70,
    color: 'rgb(254, 189, 87)',
  };

  var semicircle1 = {
    x: 250,
    y: 275,
    vx: -3,
    vy: -8,
    radius: 80,
    color: 'rgb(245, 37, 36)',
    angle: -0.3,
  }

  var semicircle2 = {
    x: 255,
    y: 125,
    vx: -2,
    vy: -5,
    radius: 50,
    color: 'rgb(245, 37, 36)',
    angle: 0.3,
  }

  function resetCircles() {
    ball = {
      x: 260,
      y: 250,
      vx: -3,
      vy: -10,
      radius: 50,
      color: 'rgb(255, 145, 75)',
    };
    ball2 = {
      x: 305,
      y: 445,
      vx: 4,
      vy: -10,
      radius: 70,
      color: 'rgb(254, 189, 87)',
    };
  
    semicircle1 = {
      x: 250,
      y: 275,
      vx: -3,
      vy: -8,
      radius: 80,
      color: 'rgb(245, 37, 36)',
      angle: -0.3,
    }
  
    semicircle2 = {
      x: 255,
      y: 125,
      vx: -2,
      vy: -5,
      radius: 50,
      color: 'rgb(245, 37, 36)',
      angle: 0.3,
    }
  }

  function drawCircle() {
    clear();
    ctx.beginPath();
    ctx.arc(ball.x, ball.y, ball.radius, 0, Math.PI * 2, true);
    ctx.fillStyle = ball.color;
    ctx.fill();
    ctx.closePath();

    ctx.beginPath();
    ctx.arc(ball2.x, ball2.y, ball2.radius, 0, Math.PI * 2, true);
    ctx.fillStyle = ball2.color;
    ctx.fill();
    ctx.closePath();

    draw_semi_circles(semicircle1.x, semicircle1.y, semicircle1.radius, semicircle1.color, semicircle1.angle);
    draw_semi_circles(semicircle2.x, semicircle2.y, semicircle2.radius, semicircle2.color, semicircle2.angle);
  }

  function clear() {
    draw_square(50, 50, 500, 500);
    //draw_semi_circles(250, 275, 80, "rgb(245, 37, 36)", -0.3);
    //draw_semi_circles(270, 125, 50, "rgb(245, 37, 36)", 0.3);
  }

  function drawArt() {
    counter += 1;
    if (counter >= 600) {
      counter = 0
      running = false;
      resetCircles();

      drawCircle();
    }
    else {
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      clear();
      //ball 1
      ball.x += ball.vx;
      ball.y += ball.vy;
      if (
        ball.y + ball.vy > canvas.height - 100 - ball.radius ||
        ball.y + ball.vy < 0
      ) {
        ball.vy = -ball.vy;
      }
      if (ball.x + ball.vx > canvas.width - 100 || ball.x + ball.vx < 100) {
        ball.vx = -ball.vx;
      }
  
      ball.vy *= 0.99;
      ball.vy += 0.25;
      ball.x *= 0.99;
      ball.x += 0.25;
  
      if (ball.y > 500 - ball.radius) {
        ball.y = 500 - ball.radius;
      }
  
      ctx.beginPath();
      ctx.arc(ball.x, ball.y, ball.radius, 0, Math.PI * 2, true);
      ctx.fillStyle = ball.color;
      ctx.fill();
      ctx.closePath();
  
      //ball 2
      ball2.x += ball2.vx;
      ball2.y += ball2.vy;
  
      if (
        ball2.y + ball2.vy > canvas.height - 100 - ball2.radius ||
        ball2.y + ball2.vy < 0
      ) {
        ball2.vy = -ball2.vy;
      }
      if (ball2.x + ball2.vx > canvas.width || ball2.x + ball2.vx < 100) {
        ball2.vx = -ball2.vx;
      }
  
      ball2.vy *= 0.99;
      ball2.vy += 0.25;
      ball2.x *= 0.99;
      ball2.x += 0.25;
  
      if (ball2.y > 500 - ball2.radius) {
        ball2.y = 500 - ball2.radius;
      }
  
      ctx.beginPath();
      ctx.arc(ball2.x, ball2.y, ball2.radius, 0, Math.PI * 2, true);
      ctx.fillStyle = ball2.color;
      ctx.fill();
      ctx.closePath();
  
      //semicircle 1
      semicircle1.x += semicircle1.vx;
      semicircle1.y += semicircle1.vy;
  
      if (
        semicircle1.y + semicircle1.vy > canvas.height - 100 - semicircle1.radius ||
        semicircle1.y + semicircle1.vy < 0
      ) {
        semicircle1.vy = -semicircle1.vy;
        if (Math.abs(semicircle1.vy) > 3) {
          semicircle1.angle = -(semicircle1.angle-Math.random()*0.5);
        }
        
      }
      if (semicircle1.x + semicircle1.vx > canvas.width || semicircle1.x + semicircle1.vx < 100) {
        semicircle1.vx = -semicircle1.vx;
        semicircle1.angle = -(semicircle1.angle-Math.random()*0.5);
      }
  
      semicircle1.vy *= 0.99;
      semicircle1.vy += 0.25;
      semicircle1.x *= 0.99;
      semicircle1.x += 0.25;
  
      if (semicircle1.y > 500 - semicircle1.radius) {
        semicircle1.y = 500 - semicircle1.radius;
      }
  
      ctx.beginPath();
      ctx.arc(semicircle1.x, semicircle1.y, semicircle1.radius, semicircle1.angle, semicircle1.angle + Math.PI, false);
      ctx.fillStyle = semicircle1.color;
      ctx.fill();
  
  
  
      //semicircle 2
      semicircle2.x += semicircle2.vx;
      semicircle2.y += semicircle2.vy;
  
      if (
        semicircle2.y + semicircle2.vy > canvas.height - 100 - semicircle2.radius ||
        semicircle2.y + semicircle2.vy < 0
      ) {
        semicircle2.vy = -semicircle2.vy;
        if (Math.abs(semicircle2.vy) > 3) {
          semicircle2.angle = -semicircle2.angle;
        }
      }
      if (semicircle2.x + semicircle2.vx > canvas.width || semicircle2.x + semicircle2.vx < 100) {
        semicircle2.vx = -semicircle2.vx;
        semicircle2.angle = -(semicircle2.angle-0.1);
      }
  
      semicircle2.vy *= 0.99;
      semicircle2.vy += 0.25;
      semicircle2.x *= 0.99;
      semicircle2.x += 0.25;
  
      if (semicircle2.y > 500 - semicircle2.radius) {
        semicircle2.y = 500 - semicircle2.radius;
      }
  
      ctx.beginPath();
      ctx.arc(semicircle2.x, semicircle2.y, semicircle2.radius, semicircle2.angle, semicircle2.angle + Math.PI, false);
      ctx.fillStyle = semicircle2.color;
      ctx.fill();
      if (running) {
        raf = window.requestAnimationFrame(drawArt);
      }
    }
    
    
    
  }

  canvas.addEventListener("click", function (e) {
    if (!running) {
      raf = window.requestAnimationFrame(drawArt);
      running = true;
    }
  });

  drawCircle();

  }

