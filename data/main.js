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
    if ((this.readyState == 4) && (this.status == 200)) {
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

const colorSwitch = document.querySelector("#iconBulb");
function changeTheme(ev){
    let elementStyle = window.getComputedStyle(colorSwitch);
    let elementColor = elementStyle.getPropertyValue('--icon-color');
    //console.log(elementColor);
    if(elementColor==' #6e6e6e') {
    //console.log('cambiar a dark');
    document.documentElement.setAttribute('theme', 'dark');
    } else {
    //console.log('cambiar a light');
    document.documentElement.setAttribute('theme', 'light');
    }
};
    colorSwitch.addEventListener('click', changeTheme);