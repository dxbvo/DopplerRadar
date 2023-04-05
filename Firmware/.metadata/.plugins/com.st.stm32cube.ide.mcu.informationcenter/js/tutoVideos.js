/*******************************************************************************
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved. This program and the accompanying materials
 * is the property of STMicroelectronics and must not be
 * reproduced, disclosed to any third party, or used in any
 * unauthorized manner without written consent.
 *******************************************************************************/
let oldSelectiCate = 0;
let oldSelectyVideos = 0;

function ProcessCategoriesJson(videosTab, server) {
	for (i = 0; i < videosTab.length; i++) {
		let div = document.createElement("div");
		div.setAttribute('class', 'videoCat');
		document.getElementById('videoDiv').appendChild(div);
		let span = document.createElement("span");
		span.setAttribute('class', 'container__box__link--text');
		span.innerHTML += videosTab[i].category;
		div.appendChild(span);
		let table = document.createElement("table");
		table.setAttribute('id', 'videoTable'+i);
		table.setAttribute('class', 'videoCatTable');
		div.appendChild(table);
		let trT = document.createElement("tr");
		table.appendChild(trT);
		FillJSonPlaylist(videosTab[i].id, i, server);
	}
}

function ProcessDuration(videoDuration, idElemI, idElemeY){
	match = videoDuration.match(/PT(\d+H)?(\d+M)?(\d+S)?/);
	match = match.slice(1).map(function(x) {
	    if (x != null) {
	        return x.replace(/\D/, '');
	    }
	});
	minutes = (parseInt(match[1]) || 0);
	seconds = (parseInt(match[2]) || 0);
	durationStr = minutes + ':' + ("0" + seconds).slice(-2);

	if(idElemI == 0 && idElemeY == 0){
		// add on big pictures
		let span = document.getElementById('curVidDuration');
		span.innerText = durationStr;
	}
	// also on the little one
	let span = document.getElementById('dur' + idElemI + idElemeY);
	if(document.getElementById('dur' + idElemI + idElemeY))
		span.innerText = durationStr;
}

function ProcessPlaylistJson(dataPlaylist, i, server){
		// if cn server, suppress empty url videos (videoUrlCN = null)
		const removeItems = [];
		if(server === "stmcu.com.cn"){
			for(d = 0; d < dataPlaylist.length; d++) {
				if(dataPlaylist[d].snippet.resourceId.videoUrlCN == "null"){
					removeItems[removeItems.length] = d;
				}
			}
			for(r = 0; r < removeItems.length; r++){
				dataPlaylist.splice(removeItems[r], 1);
			}
		}
		let tr = document.createElement("tr");
		document.getElementById('videoTable'+i).appendChild(tr);
		// center area
		// by default we show the 1st videos of the 1st category
		if(i == 0){
			// add Title and description to the center area
			let trarea = document.getElementById('curVideoDesc');
			let parea = document.createElement("p");
			parea.setAttribute('align', 'left');
			parea.setAttribute('id', 'pCenter');
			let descript;
			descript = dataPlaylist[0].snippet.description;
			descript = descript.replace(/\n/g, '<br />');
			// if url in the text to it clickable
			descript = DoUrlClickable(descript);
			parea.innerHTML  = '<p class=descTitle>' + dataPlaylist[0].snippet.title + '</p> <br> <p class=descContent>' + descript + '</p>';
			trarea.appendChild(parea);
			// add a picture to the center area
			let imgarea = document.getElementById('curVideoPic');
			if(server === "stmcu.com.cn"){
				imgarea.setAttribute('src', "data:image/png;base64," + dataPlaylist[0].snippet.resourceId.encodedThumbnailmq);
			} else {
				imgarea.setAttribute('src', dataPlaylist[0].snippet.thumbnails.medium.url);
			}
			// add the link to the video to the center area
			let areaarea = document.getElementById('videoLink');
			// keep this line to remember that it is not in full screen but description visible
			// areaarea.setAttribute('href', "http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url=https://www.youtube.com/watch?v%3D" + dataPlaylist[0].snippet.resourceId.videoId);
			if(server === "stmcu.com.cn"){
				areaarea.setAttribute('href', "http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url=" + dataPlaylist[0].snippet.resourceId.videoUrlCN);
			} else {
				areaarea.setAttribute('href', "http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url=https://www.youtube.com/embed/" + dataPlaylist[0].snippet.resourceId.videoId + "?cc_load_policy%3D1%26autoplay%3D1%26list%3D" + dataPlaylist[0].snippet.playlistId);
			}
			// add on big pictures
			ProcessDuration(dataPlaylist[0].snippet.duration, 0, 0);
		}
		// fill videos row
		for (y = 0; y < dataPlaylist.length; y++) {
			let td = document.createElement("td");
			tr.appendChild(td);
			let div = document.createElement("div");
			div.setAttribute('style', 'width:200px; position:relative;');
			td.appendChild(div);
			let img = document.createElement("img");
			if(i == 0 && y == 0){
				img.setAttribute('border', '0');
				img.setAttribute('style', "box-shadow:0px 0px 22px 8px #3CB4E6; border-radius: 6px;");
			}
			else{
				img.setAttribute('border', '0');
				img.setAttribute('style', "border-radius: 6px;");
			}
			img.setAttribute('class', 'thumbnail');
			img.setAttribute('id', "img" + i + y);
			if(server === "stmcu.com.cn"){
				img.setAttribute('src', "data:image/png;base64," + dataPlaylist[y].snippet.resourceId.encodedThumbnail);
			} else {
				img.setAttribute('src', dataPlaylist[y].snippet.thumbnails.default.url);
			}
			img.setAttribute('position', 'absolute');
			img.setAttribute('align', 'right');
			img.setAttribute('usemap', "#click-map" + i + y);
			div.appendChild(img);
			if(dataPlaylist[y].snippet.resourceId.status == "new"){
				let a = document.createElement("a");
				a.setAttribute('class', "newWrapper");
				div.appendChild(a);
			}
			let span = document.createElement("span");
			span.setAttribute('class', 'vidDuration');
			span.setAttribute('id', "dur" + i + y);
			span.innerText = "N/A";
			div.appendChild(span)
			let map = document.createElement("map");
			map.setAttribute('name', "click-map" + i + y);
			img.appendChild(map);
			let descriptImg;
			let txt;
			txt = dataPlaylist[y].snippet.description;
			txt = txt.replace(/\n/g, '<br />');
			// if url in the text to it clickable
			txt = DoUrlClickable(txt);
			descriptImg  = '<p class=descTitle>' + dataPlaylist[y].snippet.title + '</p> <br> <p class=descContent>' + txt + '</p>';
			let area = document.createElement("area");
			area.setAttribute('id', "area" + i + y);
			area.setAttribute('data-txtImg', descriptImg);
			if(server === "stmcu.com.cn"){
				area.setAttribute('data-urlImg', "data:image/png;base64," + dataPlaylist[y].snippet.resourceId.encodedThumbnailmq);
			} else {
				area.setAttribute('data-urlImg', dataPlaylist[y].snippet.thumbnails.medium.url);
			}
			area.setAttribute('data-idImg', dataPlaylist[y].snippet.resourceId.videoId);
			area.setAttribute('data-urlCNImg', dataPlaylist[y].snippet.resourceId.videoUrlCN);
			area.setAttribute('shape', 'shape');
			area.setAttribute('coords', '0, 0, 120, 90');
			if(server === "stmcu.com.cn"){
				area.setAttribute('onclick', "ChangeCenterArea(" + i + "," + y + "," + "\"stmcu.com.cn\"" + ")");
			} else {
				area.setAttribute('data-idPlaylist', dataPlaylist[y].snippet.playlistId);
				area.setAttribute('onclick', "ChangeCenterArea(" + i + "," + y + "," + "\"Youtube\"" + " )");
			}
			area.setAttribute('href', "#!");
			ProcessDuration(dataPlaylist[y].snippet.duration, i, y);
			map.appendChild(area);
		}
		// Fill title row
		let trt = document.createElement("tr");
		document.getElementById('videoTable'+i).appendChild(trt);
		for (x = 0; x < dataPlaylist.length; x++) {
			let tdt = document.createElement("td");
			trt.appendChild(tdt);
			let divt = document.createElement("div");
			divt.setAttribute('style', 'width:200px');
			tdt.appendChild(divt);
			let pt = document.createElement("p");
			pt.setAttribute('class', 'container__title--text');
			pt.innerHTML = dataPlaylist[x].snippet.title;
			divt.appendChild(pt);
		}
	}

function ChangeCenterArea(iCate, yVideos, server){
	// update center area
	// suppress old border on the last selected picture
	let imgOld = document.getElementById('img' + oldSelectiCate + oldSelectyVideos);
	imgOld.setAttribute('border', '0');
	imgOld.setAttribute('style', 'border-radius: 6px;');
	// add border on the selected picture
	oldSelectiCate = iCate;
	oldSelectyVideos = yVideos;
	let img = document.getElementById('img' + iCate + yVideos);
	img.setAttribute('border', '0');
	img.setAttribute('style', "box-shadow:0px 0px 22px 8px #3CB4E6; border-radius: 6px;");
	// add Title and descriptino to the center area
	let area = document.getElementById('area' + iCate + yVideos);
	let text = area.getAttribute('data-txtImg');
	let elt = document.getElementById('pCenter');
	elt.innerHTML = text;
	// add a picture to the center area	
	let imgarea = document.getElementById('curVideoPic');
	imgarea.setAttribute('src', area.getAttribute('data-urlImg'));
	// add the link to the video to the center area
	let areaarea = document.getElementById('videoLink');
	// keep this line to remember that it is not in full screen but description visible
	// areaarea.setAttribute('href', "http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url=https://www.youtube.com/watch?v%3D" + area.getAttribute('data-idImg'));
	if(server === "stmcu.com.cn"){
		areaarea.setAttribute('href', "http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url=" + area.getAttribute('data-urlCNImg'));
	} else {
		let playlistId = area.getAttribute('data-idPlaylist');
		areaarea.setAttribute('href', "http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url=https://www.youtube.com/embed/" + area.getAttribute('data-idImg') + "?cc_load_policy%3D1%26autoplay%3D1%26list%3D" + playlistId);
	}
	let span = document.getElementById('curVidDuration');
	span.innerText = document.getElementById('dur' + iCate + yVideos).innerText;
}

function FillJSonPlaylist(id, i, server){
	baseurl = 'https://sw-center.st.com/stm32cubeide/videos/2.0/';
	url = baseurl + id + '.json';
	$.ajax({
		url: url,
		dataType: 'json',
		type: 'GET',
		crossDomain: true,
		success: function(dataremote){
			// Adapt Marcom Json format
			if (dataremote.hasOwnProperty('items')) {
				if (Array.isArray(dataremote.items)) {
					dataremote = dataremote.items;
				} 
				else{
					dataremote = [ dataremote.items ];
				}
				console.log(url);
				console.log(dataremote);
				ProcessPlaylistJson(dataremote, i, server);
			} else {
				console.log('[ERROR] unknown playlist:' + id + ' youtube format');
			}
		},
		error: function(XHR, textStatus, errorThrown) {
		console.log('FillJSonPlaylist');
		console.log(textStatus);
		console.log(errorThrown);
    }
	 });
}

function DoUrlClickable(text) {
    var urlRegex =/(\b(https?|ftp|file):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig;
    return text.replace(urlRegex, function(url) {
        return '<a href="' + url + '">' + url + '</a>';
    });
}

$('select').on('change', function (e) {
    $('select option').prop('disabled', false);
    var optionSelected = $("option:selected", this);
    var valueSelected = this.value;
    // suppress Current videos div and videos Div
    var curvid = document.getElementById('curVideoDesc');
    while (curvid.firstChild) {
		curvid.removeChild(curvid.firstChild);
	}
    var vid = document.getElementById('videoDiv');
    while (vid.firstChild) {
		vid.removeChild(vid.firstChild);
	}
    loadPage();
});

loadPage();

function loadPage(){
	// remote pictures
	baseurl = 'https://sw-center.st.com/stm32cubeide/videos/2.0/';
	url = baseurl + 'videosyoutube.json';
	var e = document.getElementById("server_select_box");
	var str = e.options[e.selectedIndex].value;
	if (str==="Youtube"){
		url = baseurl + 'videosyoutube.json';
	}
	if (str==="stmcu.com.cn"){
		url = baseurl + 'videoscn.json';
	}
	$.ajax({
		url: url,
		dataType: 'json',
		type: 'GET',
		crossDomain: true,
		success: function(dataremote){
			// Adapt Videos Json format
			if (dataremote.hasOwnProperty('items')) {
				if (Array.isArray(dataremote.items)) {
					dataremote = dataremote.items;
				}
				else {
					dataremote = [ dataremote.items ];
				}
				console.log(url);
				console.log(dataremote);
				ProcessCategoriesJson(dataremote, str);
			}
			else {
				ProcessCategoriesJson(JSON.parse(items), str);
				console.log('[ERROR] unknown videos list format');
			}
		},
		error: function(XHR, textStatus, errorThrown) {
			let imgarea = document.getElementById('curVideoPic');
			imgarea.setAttribute('border', 0);
			let trarea = document.getElementById('curVideoDesc');
			let parea = document.createElement("p");
			parea.setAttribute('align', 'left');
			parea.setAttribute('id', 'pCenter');
			parea.innerHTML = '<b>Network Error</b>' + '<br><br>Could not complete the operation due to an error';
			trarea.appendChild(parea);
			console.log(textStatus);
			console.log(errorThrown);
	    }
	 });
}