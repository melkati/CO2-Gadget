const char MAIN_page[] PROGMEM = R"rawliteral(
<!-- See https://medium.com/@pppped/how-to-code-a-responsive-circular-percentage-chart-with-svg-and-css-3632f8cd7705 -->
<!DOCTYPE html>
<html>

<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <!-- <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
    integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous"> -->
  <style>
    .flex-wrapper {
  display: flex;
  flex-flow: row nowrap;
}

.single-chart {
  width: 33%;
  justify-content: space-around ;
}

.circular-chart {
  display: block;
  margin: 10px auto;
  max-width: 80%;
  max-height: 250px;
}

.circle-bg {
  fill: none;
  stroke: #eee;
  stroke-width: 3.8;
}

.circle {
  fill: none;
  stroke-width: 2.8;
  stroke-linecap: round;
  animation: progress 1s ease-out forwards;
}

@keyframes progress {
  0% {
    stroke-dasharray: 0 100;
  }
}

.circular-chart.co2 .circle {
  stroke: #ff9f00;
}

.circular-chart.temp .circle {
  stroke: #4CC790;
}

.circular-chart.humi .circle {
  stroke: #3c9ee5;
}

.percentage {
  fill: #666;
  font-family: sans-serif;
  font-size: 0.5em;
  text-anchor: middle;
}
/*smartphone styles*/
@media only screen and (max-width: 639px) {
    .flex-wrapper {
       flex-flow: column wrap;
       align-items: center;
    }
    .single-chart {
        hight: 33%;
    }
}
  </style>
</head>

<body>

  <div class="flex-wrapper">
    <div class="single-chart">
      <svg viewBox="0 0 36 36" class="circular-chart co2">
        <path class="circle-bg"
          d="M18 2.0845
            a 15.9155 15.9155 0 0 1 0 31.831
            a 15.9155 15.9155 0 0 1 0 -31.831"
        />
        <path class="circle"          
          stroke-dasharray="50, 100"
          d="M18 2.0845
            a 15.9155 15.9155 0 0 1 0 31.831
            a 15.9155 15.9155 0 0 1 0 -31.831"
        />
        <text x="18" y="20.35" class="percentage" id="CO2Value"> Init...</text>
        <!-- <text x="18" y="20.35" class="percentage">
          <tspan dy=".6em">CO2</tspan>
          <tspan dx="0" dy="1.2em" id="CO2Value">Init...</tspan>
          <tspan dx="0" dy="1.2em">ppm</tspan>
        </text> -->
      </svg>
    </div>
    
    <div class="single-chart">
      <svg viewBox="0 0 36 36" class="circular-chart temp">
        <path class="circle-bg"
          d="M18 2.0845
            a 15.9155 15.9155 0 0 1 0 31.831
            a 15.9155 15.9155 0 0 1 0 -31.831"
        />
        <path class="circle"
          stroke-dasharray="60, 100"
          d="M18 2.0845
            a 15.9155 15.9155 0 0 1 0 31.831
            a 15.9155 15.9155 0 0 1 0 -31.831"
        />
        <text x="18" y="20.35" class="percentage" id="TempValue"> Init...</text>
      </svg>
    </div>
  
    <div class="single-chart">
      <svg viewBox="0 0 36 36" class="circular-chart humi">
        <path class="circle-bg"
          d="M18 2.0845
            a 15.9155 15.9155 0 0 1 0 31.831
            a 15.9155 15.9155 0 0 1 0 -31.831"
        />
        <path class="circle"
          stroke-dasharray="90, 100"
          d="M18 2.0845
            a 15.9155 15.9155 0 0 1 0 31.831
            a 15.9155 15.9155 0 0 1 0 -31.831"
        />
        <text x="18" y="20.35" class="percentage" id="HumValue"> Init...</text>
      </svg>
    </div>
  </div>
  <script>

    setInterval(function () {
      // Call a function repetatively with 15 Second interval
      getCO2Data();
    }, 15000); // 15000mS  update rate

    setInterval(function () {
      // Call a function repetatively with 60 Second interval
      getTemperatureData();
    }, 60000); // 60000mS  update rate

    setInterval(function () {
      // Call a function repetatively with 60 Second interval
      getHumidityData();
    }, 60000); // 60000mS  update rate

    function getCO2Data() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.querySelector('text#CO2Value').textContent=this.responseText;
          // document.getElementById("text#CO2Value").textContent = this.responseText;
          // document.getElementById("CO2Value").innerHTML =
            // this.responseText;
        }
      };
      xhttp.open("GET", "readCO2", true);
      xhttp.send();
    }

    function getTemperatureData() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("TempValue").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "readTemperature", true);
      xhttp.send();
    }

    function getHumidityData() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("HumValue").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "readHumidity", true);
      xhttp.send();
    }
  </script>
</body>

</html>
)rawliteral";


const char SIMPLE_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <!-- <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
    integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous"> -->
  <style>
    .card {
      max-width: 800px;
      min-width: 310px;
      min-height: 250px;
      background: #dddf9a;
      padding: 30px;
      box-sizing: border-box;
      color: hsl(0, 0%, 20%);
      margin: 20px;
      box-shadow: 0px 2px 18px -4px rgba(0, 0, 0, 0.75);
    }
  </style>
</head>

<body>

  <div class="card">
    <h1>CO2 Gadget</h1>
    <h2>Air Quality Data</h2>
      <h3>CO2: <span id="CO2Value">Initializing...</span> ppm</h3>
      <h3>Temp: <span id="TempValue">Initializing...</span> &deg;C</h3>
      <h3>Hum: <span id="HumValue">Initializing...</span> %</h3>
      <br><a href="https://emariete.com">eMariete.com</a>
  </div>
  <script>

    setInterval(function () {
      // Call a function repetatively with 2 Second interval
      getCO2Data();
    }, 2000); //2000mSeconds update rate

    setInterval(function () {
      // Call a function repetatively with 2 Second interval
      getTemperatureData();
    }, 2000); //50000mSeconds update rate

    setInterval(function () {
      // Call a function repetatively with 2 Second interval
      getHumidityData();
    }, 2000); //50000mSeconds update rate

    function getCO2Data() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("CO2Value").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "readCO2", true);
      xhttp.send();
    }

    function getTemperatureData() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("TempValue").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "readTemperature", true);
      xhttp.send();
    }

    function getHumidityData() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("HumValue").innerHTML =
            this.responseText;
        }
      };
      xhttp.open("GET", "readHumidity", true);
      xhttp.send();
    }
  </script>
</body>

</html>
)rawliteral";

