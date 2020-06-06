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
  $("#form-red").val(colorWheel.color.rgba.r);
  $("#form-green").val(colorWheel.color.rgba.g);
  $("#form-blue").val(colorWheel.color.rgba.b);
  $("#form-alpha").val(parseInt(colorWheel.color.rgba.a * 255));
});