var meat_div;
var meatChart;
var ambient_div;
var ambientChart;
var baseOption;
var reference;

function setup_guages() {
	meat_div = document.getElementById("meat_gauge");
	meatChart = echarts.init(meat_div);
	ambient_div = document.getElementById("ambient_gauge");
	ambientChart = echarts.init(ambient_div);
	if (meat_div.offsetWidth < meat_div.offsetHeight) {
		reference = meat_div.offsetWidth;
	} else {
		reference = meat_div.offsetHeight;
	}
	baseOption = {
		baseOption: {
			series: [
				{
					type: "gauge",
					axisLine: {
						lineStyle: {
							width: reference / 20,
						},
					},
					axisTick: {
						show: true,
						distance: -reference / 60,
						length: reference / 60,
						lineStyle: {
							width: 2,
							color: "#999",
						},
					},
					splitLine: {
						length: -reference / 20,
						distance: 0,
						lineStyle: {
							width: 2,
							color: "#999",
						},
					},
					axisLabel: {
						distance: -10,
						color: "#999",
						fontSize: reference / 30,
					},
					anchor: {
						show: true,
						showAbove: true,
						size: reference / 15,
						itemStyle: {
							borderWidth: 0,
							color: "#f0f0f0",
						},
					},
					pointer: {
						itemStyle: {
							color: "#f0f0f0",
						},
					},
					detail: {
						valueAnimation: false,
						fontSize: reference / 15,
						color: "inherit",
						offsetCenter: [0, "50%"],
						formatter: "{value} °C",
					},
					data: [
						{
							name: "MEAT",
							value: 0.0,
						},
					],
					endAngle: -70,
					startAngle: 250,
					title: {
						offsetCenter: [0, "30%"],
						fontSize: reference / 20,
						color: "#f0f0f0",
					},
				},
			],
		},
	};
	meatChart.setOption(baseOption);
	ambientChart.setOption(baseOption);

	meatOption = {
		baseOption: {
			series: [
				{
					axisLine: {
						lineStyle: {
							color: [
								[0.48, "#646464"],
								[0.51, "#b03f3e"], //Beef: Rare
								[0.54, "#e16168"], //Beef: Medium rare
								[0.58, "#f07b6e"], //Beef: Medium
								[0.62, "#a5531c"], //Beef: Well Done
								[0.63, "#646464"],
								[0.68, "#FC8EAC"],
								[0.73, "#646464"],
								[0.78, "#ffc800"],
								[1, "#646464"],
							],
						},
					},
					data: [
						{
							name: "MEAT",
							value: 0.0,
						},
					],
					max: 100,
					splitNumber: 20,
				},
			],
		},
	};
	ambientOption = {
		baseOption: {
			series: [
				{
					axisLine: {
						lineStyle: {
							color: [
								[0.454545, "#646464"],
								[0.545454, "#f0f0f0"],
								[1, "#646464"],
							],
						},
					},
					data: [
						{
							name: "HEAT",
							value: 0.0,
						},
					],
					max: 220,
					splitNumber: 22,
				},
			],
		},
	};
	meatChart.setOption(meatOption);
	ambientChart.setOption(ambientOption);
}

function get_data() {
	var request = new XMLHttpRequest();

	request.addEventListener("readystatechange", () => {
		if (request.readyState == 4 && request.status == 200) {
			var data = request.responseText.split(",");
			var meat_data = data[0];
			var ambient_data = data[1];
			meatChart.setOption({
				series: [
					{
						data: [
							{
								name: "MEAT",
								value: Number(meat_data),
							},
						],
					},
				],
			});
			ambientChart.setOption({
				series: [
					{
						data: [
							{
								name: "HEAT",
								value: Number(ambient_data),
							},
						],
					},
				],
			});
			console.log(data);
		} else if (request.readyState == 4) {
			console.log(request.status);
		}
	});
	request.open("GET", "data", true);
	request.setRequestHeader("Cache-Control", "no-cache");
	request.send();
}

setup_guages();
get_data();

window.addEventListener("resize", function () {
	"use strict";
	// window.location.reload();
	meatChart.resize();
	ambientChart.resize();
	setup_guages();
});

setInterval(function () {
	get_data();
}, 500);
