// Prehliadač schém: posúvanie ťahaním, zoom tlačidlami / Ctrl + koliesko / dvoma prstami,
// celá obrazovka. Použitie: <div class="sv" data-src="schema.svg" data-alt="popis"></div>
(function () {
  function viewer(box) {
    var img = new Image();
    img.src = box.getAttribute("data-src");
    img.alt = box.getAttribute("data-alt") || "";
    img.draggable = false;

    var stage = document.createElement("div");
    stage.className = "sv-stage";
    stage.appendChild(img);

    var bar = document.createElement("div");
    bar.className = "sv-bar";
    bar.innerHTML =
      '<button type="button" data-a="in" title="Zoom in">+</button>' +
      '<button type="button" data-a="out" title="Zoom out">−</button>' +
      '<button type="button" data-a="fit" title="Fit">⤢</button>' +
      '<button type="button" data-a="full" title="Fullscreen">⛶</button>' +
      '<span class="sv-zoom"></span>';

    box.appendChild(stage);
    box.appendChild(bar);

    var s = 1, x = 0, y = 0, minS = 0.05, maxS = 8;
    var zoomLabel = bar.querySelector(".sv-zoom");

    function apply() {
      img.style.transform = "translate(" + x + "px," + y + "px) scale(" + s + ")";
      zoomLabel.textContent = Math.round(s * 100) + " %";
    }

    // na začiatok: na šírku (aby bol text čitateľný), vysoké schémy od vrchu
    function fit(whole) {
      var W = stage.clientWidth, H = stage.clientHeight;
      var w = img.naturalWidth || 1000, h = img.naturalHeight || 1000;
      s = whole ? Math.min(W / w, H / h) : W / w;
      minS = Math.min(W / w, H / h) * 0.5;
      x = (W - w * s) / 2;
      y = h * s < H ? (H - h * s) / 2 : 0;
      apply();
    }

    function zoomAt(f, cx, cy) {
      var ns = Math.max(minS, Math.min(maxS, s * f));
      f = ns / s;
      x = cx - (cx - x) * f;
      y = cy - (cy - y) * f;
      s = ns;
      apply();
    }

    function center() {
      return [stage.clientWidth / 2, stage.clientHeight / 2];
    }

    img.addEventListener("load", function () { fit(false); });
    window.addEventListener("resize", function () { fit(false); });

    bar.addEventListener("click", function (e) {
      var a = e.target.getAttribute && e.target.getAttribute("data-a");
      var c = center();
      if (a === "in") zoomAt(1.4, c[0], c[1]);
      if (a === "out") zoomAt(1 / 1.4, c[0], c[1]);
      if (a === "fit") fit(true);
      if (a === "full") {
        if (document.fullscreenElement) document.exitFullscreen();
        else if (box.requestFullscreen) box.requestFullscreen();
        else box.classList.toggle("sv-max");   // iPhone nemá Fullscreen API
      }
    });

    document.addEventListener("fullscreenchange", function () {
      setTimeout(function () { fit(false); }, 50);
    });

    // koliesko: v celej obrazovke vždy, inak len s Ctrl (aby sa dala stránka scrollovať)
    stage.addEventListener("wheel", function (e) {
      var full = document.fullscreenElement === box || box.classList.contains("sv-max");
      if (!full && !e.ctrlKey) return;
      e.preventDefault();
      var r = stage.getBoundingClientRect();
      zoomAt(e.deltaY < 0 ? 1.15 : 1 / 1.15, e.clientX - r.left, e.clientY - r.top);
    }, { passive: false });

    // ťahanie a dva prsty
    var pts = {}, last = null, lastDist = 0;
    stage.addEventListener("pointerdown", function (e) {
      stage.setPointerCapture(e.pointerId);
      pts[e.pointerId] = [e.clientX, e.clientY];
      last = [e.clientX, e.clientY];
      stage.classList.add("drag");
    });
    stage.addEventListener("pointermove", function (e) {
      if (!pts[e.pointerId]) return;
      pts[e.pointerId] = [e.clientX, e.clientY];
      var ids = Object.keys(pts);
      if (ids.length === 2) {
        var a = pts[ids[0]], b = pts[ids[1]];
        var d = Math.hypot(a[0] - b[0], a[1] - b[1]);
        var r = stage.getBoundingClientRect();
        if (lastDist) zoomAt(d / lastDist, (a[0] + b[0]) / 2 - r.left, (a[1] + b[1]) / 2 - r.top);
        lastDist = d;
        return;
      }
      x += e.clientX - last[0];
      y += e.clientY - last[1];
      last = [e.clientX, e.clientY];
      apply();
    });
    function up(e) {
      delete pts[e.pointerId];
      lastDist = 0;
      var ids = Object.keys(pts);
      if (ids.length) last = pts[ids[0]];
      else stage.classList.remove("drag");
    }
    stage.addEventListener("pointerup", up);
    stage.addEventListener("pointercancel", up);

    // dvojklik = priblíženie na miesto
    stage.addEventListener("dblclick", function (e) {
      var r = stage.getBoundingClientRect();
      zoomAt(2, e.clientX - r.left, e.clientY - r.top);
    });

    return { fit: fit };
  }

  window.SchemViewer = viewer;
  document.querySelectorAll(".sv[data-src]").forEach(viewer);
})();
