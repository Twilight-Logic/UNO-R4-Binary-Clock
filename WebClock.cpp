#include "WebClock.h"


const char binaryWebClock::binaryClockHTML[] PROGMEM = R"EOF(
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>UNO R4 Binary Clock</title>
<link rel="stylesheet" href="/style">
<script defer src="/jscript"></script>
</head>

<body>

<h2>Binary Clock (BCD)</h2>

<div class="controls">
  <button id="startBtn">Resume Matrix</button>
  <button id="stopBtn">Sleep Matrix</button>
</div>

<div id="timeText">
  <div id="timeLine">--:--:--</div>
  <div id="dateLine">-- -- ----</div>
</div>

<div class="tzPanel">
  <div class="tzTitle">
    Timezone
    <span class="badge" id="tzBadge">loading</span>
    <span class="badge" id="runBadge">running?</span>
    <button id="fmtBtn">24h</button>
  </div>

  <div class="tzRow">
    <input id="tzInput" type="text" placeholder="Europe/Paris or Asia/Dhaka">
    <button id="tzApplyBtn">Apply</button>
    <button id="tzDetectBtn">Detect my timezone</button>
    <button id="tzReloadBtn">Reload</button>
  </div>

  <div class="tzMeta" id="tzMeta">Current timezone: (loading)</div>
  <div class="tzMsg" id="tzMsg"></div>
</div>

<div class="clockWrap">
  <div class="labelsRow">
    <div class="groupLabel">HRS</div>
    <div class="sepLabel"></div>
    <div class="groupLabel">MIN</div>
    <div class="sepLabel"></div>
    <div class="groupLabel">SEC</div>
  </div>

  <div class="weights">
    <div>8</div><div>4</div><div>2</div><div>1</div>
  </div>

  <div class="row" id="clock"></div>
</div>

<div class="footer">
UNO R4 · NTP time source · WebUI Binary Clock
</div>

</body>
</html>
)EOF";


const char binaryWebClock::binaryClockCss[] PROGMEM = R"EOF(
:root{ --scale: 1.2; }

body {
  font-family: system-ui, sans-serif;
  background: #0f172a;
  color: #e5e7eb;
  padding: calc(30px * var(--scale));
}

h2 {
  margin-bottom: calc(10px * var(--scale));
  font-size: calc(24px * var(--scale));
}

.controls { margin-bottom: calc(20px * var(--scale)); }

button {
  padding: calc(10px * var(--scale)) calc(18px * var(--scale));
  margin-right: calc(10px * var(--scale));
  border: none;
  border-radius: calc(8px * var(--scale));
  background: #1e293b;
  color: white;
  cursor: pointer;
  font-size: calc(15px * var(--scale));
  transition: 0.2s;
}

button:hover { background: #334155; }

#timeText {
  margin: calc(20px * var(--scale)) 0 calc(16px * var(--scale)) 0;
  font-weight: bold;
}

#timeLine{
  font-size: calc(34px * var(--scale));
  letter-spacing: calc(2px * var(--scale));
  color: #22c55e;
  line-height: 1.1;
}

#dateLine{
  margin-top: calc(6px * var(--scale));
  font-size: calc(18px * var(--scale));
  letter-spacing: calc(1px * var(--scale));
  color: #94a3b8; /* date en gris doux */
  font-weight: 600;
}

.clockWrap{
  display: grid;
  grid-template-columns: auto 1fr;
  grid-template-rows: auto auto;
  column-gap: calc(16px * var(--scale));
  row-gap: calc(10px * var(--scale));
  align-items: start;
  margin-top: calc(18px * var(--scale));
}

.weights{
  display: grid;
  grid-template-rows: repeat(4, calc(28px * var(--scale)));
  gap: calc(10px * var(--scale));
  margin-top: 0;
  color: #94a3b8;
  font-weight: 600;
  font-size: calc(14px * var(--scale));
  line-height: calc(28px * var(--scale));
  text-align: right;
  width: calc(20px * var(--scale));
}

.row {
  display: flex;
  gap: calc(24px * var(--scale));
  align-items: flex-start;
}

.labelsRow{
  grid-column: 2;
  grid-row: 1;
}

.weights{
  grid-column: 1;
  grid-row: 2;
}

#clock{
  grid-column: 2;
  grid-row: 2;
}

.digit {
  display: grid;
  grid-template-rows: repeat(4, calc(28px * var(--scale)));
  gap: calc(10px * var(--scale));
}

.led {
  width: calc(28px * var(--scale));
  height: calc(28px * var(--scale));
  border-radius: calc(6px * var(--scale));
  background: #020617;
  border: calc(2px * var(--scale)) solid #1e293b;
  transition: 0.2s;
}

.led.on {
  background: #22c55e;
  box-shadow:
    0 0 calc(8px * var(--scale)) #22c55e,
    0 0 calc(18px * var(--scale)) #22c55e;
}

/* Unit digits (H units, M units, S units) in red */
.unitDigit .led.on {
  background: #ef4444;
  box-shadow:
    0 0 calc(8px * var(--scale)) #ef4444,
    0 0 calc(18px * var(--scale)) #ef4444;
}

.sep { width: calc(12px * var(--scale)); }

.tzPanel{
  margin-top: calc(14px * var(--scale));
  padding: calc(14px * var(--scale));
  border: 1px solid #1e293b;
  border-radius: calc(10px * var(--scale));
  background: rgba(2,6,23,0.35);
  max-width: calc(980px * var(--scale));
}

.tzTitle{
  font-weight: 700;
  margin-bottom: calc(10px * var(--scale));
  color: #cbd5e1;
  display:flex;
  align-items:center;
  gap: calc(10px * var(--scale));
  flex-wrap: wrap;
}

.badge{
  display: inline-block;
  padding: calc(3px * var(--scale)) calc(10px * var(--scale));
  border-radius: 999px;
  background: #1e293b;
  color: #e5e7eb;
  font-size: calc(12px * var(--scale));
}

.tzRow{
  display:flex;
  gap: calc(10px * var(--scale));
  flex-wrap:wrap;
  align-items:center;
}

.tzRow input{
  padding: calc(10px * var(--scale));
  border-radius: calc(8px * var(--scale));
  border: 1px solid #334155;
  background: #0b1220;
  color: #e5e7eb;
  min-width: calc(320px * var(--scale));
  font-size: calc(14px * var(--scale));
  outline:none;
}

.tzMeta{
  margin-top: calc(10px * var(--scale));
  font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, "Liberation Mono", "Courier New", monospace;
  color: #94a3b8;
  font-size: calc(13px * var(--scale));
}

.tzMsg{
  margin-top: calc(8px * var(--scale));
  font-size: calc(14px * var(--scale));
}

.footer {
  margin-top: calc(40px * var(--scale));
  color: #64748b;
  font-size: calc(14px * var(--scale));
}

.clockCol{
  display: flex;
  flex-direction: column;
}

.labelsRow{
  display: flex;
  gap: calc(24px * var(--scale));
  margin-bottom: calc(10px * var(--scale));
  color: #facc15;
  font-weight: 800;
  font-size: calc(14px * var(--scale));
  letter-spacing: calc(1px * var(--scale));
  text-transform: uppercase;
}

.groupLabel{
  width: calc((28px * var(--scale)) * 2 + (24px * var(--scale)));
  text-align: center;
}

.sepLabel{
  width: calc(12px * var(--scale));
}

/* optional but helps alignment */
.digit{
  width: calc(28px * var(--scale));
}
)EOF";



const char binaryWebClock::binaryClockJs[] PROGMEM = R"EOF(
(function(){
  "use strict";

  function qs(id){ return document.getElementById(id); }

  let use12h = (localStorage.getItem("clock12h") === "1");

  function updateFmtBtn(){
    qs("fmtBtn").textContent = use12h ? "12h" : "24h";
  }

  function setTzMsg(text, ok) {
    const el = qs("tzMsg");
    el.textContent = text || "";
    el.style.color = ok ? "#86efac" : "#fca5a5";
  }

  function setBadge(el, text, ok) {
    el.textContent = text;
    el.style.background = ok ? "#064e3b" : "#7f1d1d";
  }

  // Added function to return offset on date for loc in ±H[:mm] format.
  // Minutes only included if not zero
  function getTimezoneOffset(date, loc) {
    // Try English to get offset. If get abbreviation, use French
    let offset;
    ['en','fr'].some(lang => {
      // Get parts - can't get just timeZoneName, must get one other part at least
      let parts = new Intl.DateTimeFormat(lang, {
        minute: 'numeric',
        timeZone: loc,
        timeZoneName:'short'
      }).formatToParts(date);
      // Get offset from parts
      let tzName = parts.filter(part => part.type == 'timeZoneName' && part.value);
      // timeZoneName starting with GMT or UTC is offset - keep and stop looping
      // Otherwise it's an abbreviation, keep looping
      if (/^(GMT|UTC)/.test(tzName[0].value)) {
        offset = tzName[0].value.replace(/GMT|UTC/,'') || '+0';
        return true;
      }
    });
    // Format offset as ±HH:mm
    // Normalise minus sign as ASCII minus (charCode 45)
    let sign = offset[0] == '\x2b'? '\x2b' : '\x2d';
    let [h, m] = offset.substring(1).split(':');
    return sign + h.padStart(2, '0') + ':' + (m || '00');
  }

  async function apiGetHourMode(){
    const r = await fetch("/api/hour_mode", { method:"GET" });
    if(!r.ok) throw new Error("GET /api/hour_mode failed: " + r.status);
    return await r.json();
  }

  async function apiSetHourMode(mode){
    const url = "/api/hour_mode?mode=" + encodeURIComponent(String(mode));
    const r = await fetch(url, { method:"POST" });
    if(!r.ok) throw new Error("POST /api/hour_mode failed: " + r.status);
    return await r.json();
  }

  async function apiGetTime(){
    const r = await fetch("/api/time", { method:"GET" });
    if(!r.ok) throw new Error("GET /api/time failed: " + r.status);
    return await r.json();
  }

  async function apiPost(path){
    const r = await fetch(path, { method:"POST" });
    if(!r.ok) throw new Error("POST " + path + " failed: " + r.status);
    return await r.text();
  }

  async function apiGetTimezone(){
    const r = await fetch("/api/timezone", { method:"GET" });
    if(!r.ok) throw new Error("GET /api/timezone failed: " + r.status);
    return await r.json();
  }

  // Amended function to send time zone offset
  async function apiSetTimezone(tz) {
    let d = new Date();
    const offs = getTimezoneOffset(d, tz);
    const url = "/api/timezone?timezone=" + encodeURIComponent(tz) + "&offset=" + encodeURIComponent(offs);
    const r = await fetch(url, { method: "POST" });
    if (!r.ok) throw new Error("POST /api/timezone failed: " + r.status);
    return await r.json();
  }
  // Amended function to send time zone offset

  updateFmtBtn();

  qs("fmtBtn").onclick = async () => {
    try{
      use12h = !use12h;
      const wanted = use12h ? 12 : 24;

      const res = await apiSetHourMode(wanted);
      if(res && res.ok){
        localStorage.setItem("clock12h", use12h ? "1" : "0");
        updateFmtBtn();
        setTzMsg("Hour mode saved: " + wanted + "h", true);
      }else{
        use12h = !use12h;
        updateFmtBtn();
        setTzMsg((res && res.error) ? res.error : "Server refused hour mode", false);
      }
    }catch(e){
      use12h = !use12h;
      updateFmtBtn();
      setTzMsg(String(e), false);
    }
  };

  (async function syncHourMode(){
    try{
      const data = await apiGetHourMode();
      const hm = (data && data.hour_mode) ? Number(data.hour_mode) : 24;
      use12h = (hm === 12);
      localStorage.setItem("clock12h", use12h ? "1" : "0");
      updateFmtBtn();
    }catch(e){
      // keep localStorage
    }
  })();

  function makeDigit() {
    const d = document.createElement("div");
    d.className = "digit";
    for (let i = 0; i < 4; i++) {
      const led = document.createElement("div");
      led.className = "led";
      d.appendChild(led);
    }
    return d;
  }

  function setDigitBCD(digitEl, value) {
    const leds = digitEl.querySelectorAll(".led");
    for (let i = 0; i < 4; i++) {
      const bit = 1 << (3 - i);
      leds[i].className = "led" + ((value & bit) ? " on" : "");
    }
  }

  const clockEl = qs("clock");
  const digits = [];

  for (let i = 0; i < 6; i++) {
    const d = makeDigit();
    if (i === 1 || i === 3 || i === 5) d.classList.add("unitDigit");
    digits.push(d);
    clockEl.appendChild(d);

    if (i === 1 || i === 3) {
      const sep = document.createElement("div");
      sep.className = "sep";
      clockEl.appendChild(sep);
    }
  }

  function formatDateInline(y, mo, d){
    const months = ["January","February","March","April","May","June","July","August","September","October","November","December"];
    if(!y || !mo || !d) return "-- -- ----";
    const name = months[mo - 1] || "??";
    return String(d).padStart(2,"0") + " " + name + " " + String(y);
  }

  function formatTimeInline(hDisp, m, s, h24){
  // mode 24h
  if(!use12h){
    return String(hDisp).padStart(2,"0") + ":" +
           String(m).padStart(2,"0") + ":" +
           String(s).padStart(2,"0");
  }

  // mode 12h → hDisp est déjà 1..12
  const suffix = (h24 >= 12) ? " PM" : " AM";

  return String(hDisp).padStart(2,"0") + ":" +
         String(m).padStart(2,"0") + ":" +
         String(s).padStart(2,"0") +
         suffix;
}

  async function tick(){
    try{
      const t = await apiGetTime();
      const h24 = t.h, m = t.m, s = t.s;
      
      const hourMode = Number(t.hour_mode || 24);

      use12h = (hourMode === 12);
      localStorage.setItem("clock12h", use12h ? "1" : "0");
      updateFmtBtn();

      let hDisp = h24;
      if (hourMode === 12) {
        hDisp = h24 % 12;
        if (hDisp === 0) hDisp = 12;
      }

      const timePart = formatTimeInline(hDisp, m, s, h24);
      const datePart = formatDateInline(t.y, t.mo, t.d);
      qs("timeLine").textContent = timePart;
      qs("dateLine").textContent = datePart;

      const vals = [
        Math.floor(hDisp/10), hDisp%10,
        Math.floor(m/10), m%10,
        Math.floor(s/10), s%10
      ];
      for (let i = 0; i < 6; i++) setDigitBCD(digits[i], vals[i]);

      setBadge(qs("runBadge"), t.running ? "matrix: on" : "matrix: sleep", true);

    }catch(e){
      qs("timeLine").textContent = "API error";
      qs("dateLine").textContent = "";
    }
  }

  async function refreshTimezoneUI(){
    try{
      const data = await apiGetTimezone();
      const tz = data.timezone || "UTC";
      qs("tzMeta").textContent = "Current timezone: " + tz;
      qs("tzInput").value = tz;
      setBadge(qs("tzBadge"), tz, true);
      await apiSetTimezone(tz);
      setTzMsg("", true);
    }catch(e){
      qs("tzMeta").textContent = "Current timezone: (error)";
      setBadge(qs("tzBadge"), "error", false);
      setTzMsg(String(e), false);
    }
  }

  async function applyTimezone(){
    const tz = (qs("tzInput").value || "").trim();
    if(!tz){
      setTzMsg("Please enter a timezone like Asia/Dhaka or Europe/Paris", false);
      return;
    }
    try{
      const res = await apiSetTimezone(tz);
      if(res && res.ok){
        setTzMsg("Saved timezone: " + (res.timezone || tz), true);
        await refreshTimezoneUI();
      }else{
        setTzMsg((res && res.error) ? res.error : "Server refused timezone", false);
      }
    }catch(e){
      setTzMsg(String(e), false);
    }
  }

  async function detectMyTimezone(){
    try{
      const tz = Intl.DateTimeFormat().resolvedOptions().timeZone;
      if(!tz){
        setTzMsg("Timezone detection failed. Please type it manually.", false);
        return;
      }
      qs("tzInput").value = tz;
      await applyTimezone();
    }catch(e){
      setTzMsg(String(e), false);
    }
  }

  function updateScale() {
    const target = 1100;
    const s = Math.min(1.5, Math.max(0.8, window.innerWidth / target));
    document.documentElement.style.setProperty("--scale", String(s));
  }

  try{
    qs("startBtn").onclick = () => apiPost("/api/start").catch(()=>{});
    qs("stopBtn").onclick  = () => apiPost("/api/stop").catch(()=>{});

    qs("tzApplyBtn").onclick  = () => applyTimezone();
    qs("tzDetectBtn").onclick = () => detectMyTimezone();
    qs("tzReloadBtn").onclick = () => refreshTimezoneUI();

    qs("tzInput").addEventListener("keydown", (ev)=>{
      if(ev.key === "Enter") applyTimezone();
    });
  }catch(e){
    // ignore
  }

  window.addEventListener("resize", updateScale);
  updateScale();

  refreshTimezoneUI();
  tick();
  setInterval(tick, 1000);

})();
)EOF";


binaryWebClock::binaryWebClock(WiFiServer& serverobj, MatrixTime& matrixClock) : 
                    binClock(serverobj), matrixClk(matrixClock) {
  strncpy(_tzone, DEFAULT_TIME_ZONE, strlen(DEFAULT_TIME_ZONE));
  _hrmode = 24;
  _tz = 0;
}


void binaryWebClock::start(){
  binClock.begin();
}


bool binaryWebClock::clientDetected(){
  wclient = binClock.available();   // listen for incoming clients
  if (wclient){
    Serial.println(F("Client connected."));
    return true;
  }
  return false;
}


void binaryWebClock::clientRespond(){

  char currentLine[REQUEST_LINE_LENGTH];                      // Buffer to hold data received from the client
  char uriData[REQUEST_LINE_LENGTH];
  char postData[REQUEST_POST_SIZE];
  uint8_t lcnt = 0;
  bool rqget = false;
  bool rqpost = false;
  bool rqline = false;

  memset(currentLine, '\0', REQUEST_LINE_LENGTH); // Initialise line buffer
  memset(postData, '\0', REQUEST_POST_SIZE);      // Initialise post data buffer

  while ( wclient.connected() && (lcnt < REQUEST_LINE_LENGTH) ){  // loop while the client's connected
    delayMicroseconds(10);                    // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
    if (wclient.available()) {                // if there's bytes to read from the client,
      char c = wclient.read();                // read a byte, then
      Serial.write(c);                        // print it out to the serial monitor
      if (c == '\n') {                        // if the byte is a newline character

        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (strlen(currentLine)==0) {

          if ( rqget ) {
            if (strlen(uriData) > 1) {
              processGet(uriData);
            }else{
              sendHttpHeader(HEADER_TEXT_HTML);
              wclient.println(binaryClockHTML);
              wclient.println();
            }
            rqget = false;
            break;
          }

          if (rqpost){

            processPost(uriData, postData);
            rqpost = false;
            break;

          }

        } else {      // currentLine contains data:

          char * cmd = strtok(currentLine, " ");
          char * uri = strtok(NULL, " ");

          rqline = false;

          // Get the request type
          if (strncasecmp(cmd, "GET", 3)==0) {
            rqget = true;
            rqline = true;
          }
          if (strncasecmp(cmd, "POST", 4)==0) {
            rqpost = true;
            rqline = true;
          }
          
          // If the line is a GET or POST request
          if ( rqline ) {

            // Grab the URI data
            memset(uriData, '\0', REQUEST_LINE_LENGTH);
            strncpy(uriData, uri, strlen(uri));

          }

          // Ignore the line
          memset(currentLine, '\0', REQUEST_LINE_LENGTH);
          lcnt = 0;

        }

      } else if (c != '\r') {    // if you got anything else but a carriage return character,
        currentLine[lcnt] = c;   // add it to the end of the currentLine
        lcnt++;
      }

    }

  }

  // close the connection:
  wclient.stop();
  Serial.println("client disconnected");

}


void binaryWebClock::sendHttpHeader(uint8_t header){
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  wclient.println("HTTP/1.1 200 OK");
  switch(header) {
    case HEADER_TEXT_HTML:
      wclient.println("Content-type:text/html");
      break;
    case HEADER_APP_JSON:
      wclient.println("Content-type:application/json");
      break;
    case HEADER_TEXT_JAVASCRIPT:
      wclient.println("Content-type:text/javascript");
      break;
    case HEADER_TEXT_CSS:
      wclient.println("Content-type:text/css");
      break;
    default:
      wclient.println("Content-type:text/html");
  }
  wclient.println();
}


void binaryWebClock::processUri(char * uridata, char * elem[], char * keypairs){

  const char delim[2] = "/";
  char * token;
  char * uripath = strtok(uridata, "?");
  char * kpairs = strtok(NULL, "?");

  // Deal with key pairs
  if (kpairs != NULL) {
    // Clear keypairs
    memset(keypairs, '\0', REQUEST_LINE_LENGTH);

    // Assign new key pairs
    strncpy(keypairs, kpairs, strlen(kpairs));
  }

  // Get path elements
  elem[0] = strtok(uripath, delim);

  for (uint8_t i=1; i<REQUEST_ELEM_COUNT; i++){

    token = strtok(NULL, delim);

    if (token != NULL) {
      elem[i] = token;
    }else{
      break;
    }
  }

}


void binaryWebClock::findKeyValue(char * findkey, char * keypairs, char * keyval){

  char kprscopy[REQUEST_LINE_LENGTH];
  char * keypair;
  
  char * kpptr;
  char * kvptr;

  // We don't want to change the passed buffer so we make a copy
  memset(kprscopy, '\0', REQUEST_LINE_LENGTH);
  strncpy(kprscopy, keypairs, strlen(keypairs));

  keypair = strtok_r(kprscopy, "&", &kpptr);

  while (keypair != NULL) {

    char * key = strtok_r(keypair, "=", &kvptr);
    char * kval = strtok_r(NULL, "=", &kvptr);
    
    if (strcasecmp(findkey, key)==0) {
      strncpy(keyval, kval, strlen(kval));
      break;
    }

    keypair = strtok_r(NULL, "&", &kpptr);

  }

}


int binaryWebClock::getCharVal(const char c) {
  int cval = int(c);
  int result = 0;
  if ((cval>0x2F)&&(cval<0x3A)) result = cval-0x30;
  if ((cval>0x40)&&(cval<0x47)) result = 0x0A + (cval-0x41);
  if ((cval>0x60)&&(cval<0x67)) result = 0x0A + (cval-0x61);
  return result;
}


void binaryWebClock::uriToString(const char * uri, char * result) {

  size_t ulen = strlen(uri);
  size_t uidx = 0;
  size_t ridx = 0;
  char c;
  uint8_t cval;
  char temp[3];
  
  if (ulen == 0) return;
  
  while (uidx < ulen) { 
    c = uri[uidx];
    if (c == '%') {
      temp[0] = uri[uidx+1];
      temp[1] = uri[uidx+2];
      temp[2] = '\0';
      cval = (getCharVal(temp[0]) * 16) + (getCharVal(temp[1]));
      result[ridx] = (char)cval;
      ridx++;
      uidx = uidx + 3;
    }else{
      result[ridx] = c;
      uidx++;
      ridx++;
    }
  }

}


void binaryWebClock::processGet(char * uridata){

  char * elem[5] = {NULL};
  char keypairs[REQUEST_LINE_LENGTH];

  processUri(uridata, elem, keypairs);

  if (elem[0]==NULL) return;

  // CSS style data
  if (strcmp(elem[0], "style") == 0) {
    sendHttpHeader(HEADER_TEXT_CSS);
    wclient.println(binaryClockCss);
    return;
  }

  // JavaScript code
  if (strcmp(elem[0], "jscript") == 0) {
    sendHttpHeader(HEADER_TEXT_JAVASCRIPT);
    wclient.println(binaryClockJs);
    return;
  }

  // API calls
  if (strcmp(elem[0], "api") == 0) {
    callApiGetFunc(elem[1]);
  }

}


void binaryWebClock::processPost(char * uridata, char * postdata){

  char * elem[5] = {NULL};
  char keypairs[REQUEST_LINE_LENGTH];

  processUri(uridata, elem, keypairs);

  if (elem[0]==NULL) return;

  // API calls
  if (strcmp(elem[0], "api") == 0) {
    callApiPostFunc(elem[1], keypairs);
  }

}


void binaryWebClock::callApiGetFunc(char * func){

  if (strcmp(func, "time") == 0) {
    sendHttpHeader(HEADER_APP_JSON);
    apiGetTime();
  }

  if (strcmp(func, "hour_mode") == 0) {
    sendHttpHeader(HEADER_APP_JSON);
    apiGetHourMode();
  }

  if (strcmp(func, "timezone") == 0) {
    sendHttpHeader(HEADER_APP_JSON);
    apiGetTimeZone();
  }

}


void binaryWebClock::callApiPostFunc(char * func, char * keypairs){

  if (strcmp(func, "start") == 0) {
    apiPostMatrixStart();
    sendHttpHeader(HEADER_TEXT_HTML);
  }

  if (strcmp(func, "stop") == 0) {
    apiPostMatrixStop();
    sendHttpHeader(HEADER_TEXT_HTML);
  }

  if (strcmp(func, "hour_mode") == 0) {
    apiPostSetHourMode(keypairs);
  }

  if (strcmp(func, "timezone") == 0) {
    apiPostSetTimeZone(keypairs);
  }

}


void binaryWebClock::apiGetTime(){
  long long int unixtime = matrixClk.getTimeDate();
  RTCTime ctime;
  ctime = RTCTime(unixtime);

  char response[256];
  sprintf(  response,
            "{\"h\" : %d, \"m\" : %d, \"s\" : %d, \"y\" : %d, \"mo\" : %d, \"d\" : %d, \"running\" : %d, \"timezone\" : \"%s\", \"hour_mode\" : %d}",
            ctime.getHour(),
            ctime.getMinutes(),
            ctime.getSeconds(),
            ctime.getYear(),
            Month2int(ctime.getMonth()),
            ctime.getDayOfMonth(),
            _isrunning,
            _tzone,
            _hrmode
          );
  wclient.println(response);
}


void binaryWebClock::apiGetHourMode(){
  char response[64];
  sprintf(response, "{ \"hour_mode\" : %d }", _hrmode);
  wclient.println(response);
}


void binaryWebClock::apiGetTimeZone(){
  char response[128];
  sprintf(response, "{ \"timezone\" : \"%s\" }", _tzone);
  wclient.println(response);
}


void binaryWebClock::apiPostMatrixStart(){
  matrixClk.start();
  _isrunning = true;
}


void binaryWebClock::apiPostMatrixStop(){
  matrixClk.stop();
  _isrunning = false;
}


void binaryWebClock::apiPostSetHourMode(char * keypairs){
  char modestr[10];
  char response[64];
  char modecmd[] = "mode";

  memset(modestr, '\0', 10);

  findKeyValue(modecmd, keypairs, modestr);
  if (modestr[0] != '\0') {
    _hrmode = atoi(modestr);
    matrixClk.setHourMode(_hrmode);
  }

  sendHttpHeader(HEADER_APP_JSON);
  sprintf(response, "{ \"ok\" : true, \"hour_mode\" : %d }", _hrmode);
  wclient.println(response);

}


void binaryWebClock::apiPostSetTimeZone(char * keypairs){

  char timeZone[TIME_ZONE_LENGTH];
  char timeOffs[TIME_OFFS_LENGTH];
  char tzkey[] = "timezone";
  char tzoffs[] = "offset";
  char response[64];

  memset(timeZone, '\0', TIME_ZONE_LENGTH);
  memset(timeOffs, '\0', TIME_OFFS_LENGTH);
  findKeyValue(tzkey, keypairs, timeZone);
  findKeyValue(tzoffs, keypairs, timeOffs);

  if (timeZone[0] != '\0') {
    memset(_tzone, '\0', TIME_ZONE_LENGTH);
    memset(_toffs, '\0', TIME_OFFS_LENGTH);
    uriToString(timeZone, _tzone);
    uriToString(timeOffs, _toffs);
    matrixClk.setTimeOffset(_toffs);
  }

  sendHttpHeader(HEADER_APP_JSON);
  sprintf(response, "{ \"ok\" : true, \"timezone\" : \"%s\" }", _tzone);
  wclient.println(response);

}
