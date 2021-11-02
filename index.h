const char MAIN_page[] PROGMEM = R"=====(
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
    }, 60000); //50000mSeconds update rate

    setInterval(function () {
      // Call a function repetatively with 2 Second interval
      getHumidityData();
    }, 60000); //50000mSeconds update rate

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
)=====";