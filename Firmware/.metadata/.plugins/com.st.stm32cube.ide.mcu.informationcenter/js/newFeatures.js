/*******************************************************************************
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved. This program and the accompanying materials
 * is the property of STMicroelectronics and must not be
 * reproduced, disclosed to any third party, or used in any
 * unauthorized manner without written consent.
 *******************************************************************************/
var picturesCount = 0;
var urlPattern = new RegExp('^https?:\\/\\/');
var maxHeight=0;

function processPicturesJson(pictures, baseurl, prio) {
	console.log(pictures);
	var pic_template = document.getElementById('picture_template');
	var dot_template = document.getElementById('dot_template');
	var pic_container = document.getElementsByClassName('new__feature__container')[0];
	var dot_container = document.getElementsByClassName('dots_container')[0];
	var i;
	if(prio){
		currentIndex = picturesCount;
	}

	for (i = 0; i < pictures.length; i++) {
		console.log(pictures[i]);
		var clone = pic_template.cloneNode(true);
		clone.className = clone.className.replace(/_template/g, "");
		clone.id = '';

		var img = clone.getElementsByTagName('img')[0];
		img.onload=function() {
			var ratio = this.width/520; // see css for .new__feature__container
			maxHeight = Math.max(2+this.height/ratio, maxHeight); // 2 for padding
		};

		if (pictures[i].hasOwnProperty('image')){
			var url = pictures[i].image;
			if (urlPattern.test(url)) {
				img.src = url;
			} else {
				img.src = baseurl + url;
			}
		} else {
			/* display default error picture ? */
		}

		if (pictures[i].hasOwnProperty('link')){
			var pic_link = clone.getElementsByClassName('picture_link')[0];
			var link = pictures[i].link;
			if (!link || 0 === link.length) {
				pic_link.style.display = 'none';
			} else {
				pic_link.href="http://org.eclipse.ui.intro/runAction?pluginId=com.st.stm32cube.ide.mcu.informationcenter&class=com.st.stm32cube.ide.mcu.informationcenter.DefaultApplicationOpenerAction&url="+encodeURIComponent(link);
				var tooltip = clone.getElementsByClassName('picture_link_tooltip')[0];
				tooltip.innerHTML = 'Open ' + link;
			}
		}

		var num = clone.getElementsByClassName('number-text_template')[0];
		num.className = num.className.replace(/_template/g, "");

		if (pictures[i].hasOwnProperty('caption')){
			var caption = clone.getElementsByClassName('caption-text')[0];
			caption.innerHTML = pictures[i].caption;
		}

		pic_container.appendChild(clone);

		clone = dot_template.cloneNode(true);
		clone.className = clone.className.replace(/_template/g, "");
		clone.id = '';
		clone.style.display= '';
		clone.setAttribute('onclick', 'clearIntervalLoop();setPicture('+ picturesCount++ +')');
		dot_container.appendChild(clone);
	}

	pic_container.style.minHeight = maxHeight+"px";
}

// slider
var currentIndex = 0;
function loadSlider() {
	//var elems = document.getElementsByClassName('number-text');
	//for (i = 0; i < elems.length; i++) {
	//	elems[i].innerHTML = (i+1) + '/' + elems.length;
	//}

	showPictureAt(currentIndex);
}

function nextPicture(n) {
	showPictureAt(currentIndex += n);
}

function setPicture(n) {
	showPictureAt(currentIndex = n);
}

function showPictureAt(n) {
	var i;
	var pics = document.getElementsByClassName("picture");
	var dots = document.getElementsByClassName("dot");

	if (n >= pics.length) {currentIndex = 0}
	if (n < 0) {currentIndex = pics.length-1}
	for (i = 0; i < pics.length; i++) {
		pics[i].style.display = "none";
	}
	for (i = 0; i < dots.length; i++) {
		dots[i].className = dots[i].className.replace(" active", "");
	}
	pics[currentIndex].style.display = "block";
	dots[currentIndex].className += " active";
}

function clearIntervalLoop() {
	clearInterval(intervalId);
}

// load pictures
// local pictures
processPicturesJson(JSON.parse(data), '', false);

// remote pictures
baseurl = 'https://www.st.com/bin/st/';
url = baseurl + 'getAd.json';
$.ajax({
	url: url,
	dataType: 'json',
	type: 'GET',
	crossDomain: true,
	success: function(dataremote){
		// Adapt Marcom Json format
		if (dataremote.hasOwnProperty('Ads')) {
			if (Array.isArray(dataremote.Ads)) {
				dataremote = dataremote.Ads;
			} else {
				dataremote = [ dataremote.Ads ];
			}
			processPicturesJson(dataremote, baseurl, true);
		} else {
			console.log('[ERROR] unknown Marcom format');
		}
		loadSlider();
	},
	error: function(XHR, textStatus, errorThrown) {
		console.log(textStatus);
		console.log(errorThrown);
		loadSlider();
    }
 });

intervalId=setInterval(nextPicture, 5000, 1)
