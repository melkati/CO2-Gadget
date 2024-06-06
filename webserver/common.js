var captivePortalDebug = false;
var forcedCaptivePortalDebug = false;
var captivePortalActive = false;
var forcedCaptivePortal = false;
var captivePortalNoTimeout = false;
var forcedCaptivePortalNoTimeout = false;
var relaxedSecurity = false;

// Global variables to store the captive portal status
var captivePortalStatus = {
    captivePortalTimeLeft: 0
};

// Global variables to store the previous state
var previousData = {
    forceCaptivePortalActive: false,
    captivePortalActive: false,
    forcedCaptivePortal: false,
    captivePortalDebug: false,
    relaxedSecurity: false,
    forcedCaptivePortalDebug: false,
    captivePortalNoTimeout: false
};