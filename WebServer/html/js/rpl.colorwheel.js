var colorWheel = new iro.ColorPicker("#colorWheel", {
  layout: [
  {
    component: iro.ui.Wheel
  },
  {
    component: iro.ui.Slider,
    options: {
      sliderType: "hue"
    }
  },
  {
    component: iro.ui.Slider,
    options: {
      sliderType: "saturation"
    }
  },
  {
    component: iro.ui.Slider,
    options: {
      sliderType: "alpha"
    }
  }
  ]
});

colorWheel.on("color:change", function(color, changes){
  console.log(colorWheel.color.COLORFORMAT);
});