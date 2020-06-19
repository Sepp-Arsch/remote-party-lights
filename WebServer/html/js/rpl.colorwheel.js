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

$("#form-red").change(function() {
  colorWheel.color.setChannel("rgba", "r", $(this).val());
});

$("#form-green").change(function() {
  colorWheel.color.setChannel("rgba", "g", $(this).val());
});

$("#form-blue").change(function() {
  colorWheel.color.setChannel("rgba", "b", $(this).val());
});

$("#form-alpha").change(function() {
  colorWheel.color.setChannel("rgba", "a", $(this).val() / 255);
});

updateFormFromWheel = function() {
  $("#form-red").val(colorWheel.color.rgba.r);
  $("#form-green").val(colorWheel.color.rgba.g);
  $("#form-blue").val(colorWheel.color.rgba.b);
  $("#form-alpha").val(parseInt(colorWheel.color.rgba.a * 255));
}
colorWheel.on("input:change", updateFormFromWheel);

autoHue = function() {
	if($("#check-autoHue").is(":checked")) {
        colorWheel.color.setChannel("hsla", "h", (colorWheel.color.hsla.h + 1) % 360);
        updateFormFromWheel();
        setTimeout(autoHue, 20);
    }
};
$("#check-autoHue").change(autoHue);

resetColorForm = function() {
    colorWheel.color.reset();
    $("#form-red").val(255);
    $("#form-green").val(255);
    $("#form-blue").val(255);
    $("#form-alpha").val(255);
    $("#form-alphaMin").val(0);
}
$("#btn-white").click(resetColorForm);
$("#btn-white-save").click(function() {
    resetColorForm();
    save();
});