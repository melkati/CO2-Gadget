document.addEventListener("DOMContentLoaded", function () {
    var currentURL = window.location.href;
    if (currentURL.includes("ota.html")) {
        highlightCurrentPage();
    }
});
