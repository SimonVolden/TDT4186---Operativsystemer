
const simpleColors = ["red", "yellow", "blue", "brown", "orange", "green", "violet", "black", "pink", "white", "gray"]

function randomColorString() {
    return simpleColors[Math.floor(Math.random() * simpleColors.length)];
}

//jquery calls
$(document).ready(function () {
    $("#btn").click(function () {
        $("#doc-elem").toggle(0, function () { //https://www.educative.io/edpresso/how-to-use-toggle-in-jquery
            if ($("#doc-elem").is(":visible")) { //https://stackoverflow.com/questions/12949338/change-div-text-with-jquery-toggle
                $("#btn").text("Skjul dokumentasjon");
            } else {
                $("#btn").text("Vis dokumentasjon");
            }
        });
    });
    $("svg").hover(function () {
        // over
        $("#sail").css("opacity", "0.7"),
            colorInUse = randomColorString()
        $("#svgboat").css({ "fill": colorInUse, "stroke": colorInUse })
    }, function () {
        // out
        $("#sail").css("opacity", "1")
        $("#svgboat").css({ "fill": "brown", "stroke": "brown", "fadeIn": 1000 });
    }
    );
    $("#myCanvas").hover(function () {
        // over
        changeSailOpacity(0.5);
        changeBoatColor(randomColorString());
    }, function () {
        // out
        changeSailOpacity(1);
        changeBoatColor("brown");
    }
    );
    $("svg").click(function () {
        colorInUse = randomColorString()
        $("#svgboat").css({ "fill": colorInUse, "stroke": colorInUse });

    });
    $("#myCanvas").click(function () {
        changeBoatColor(randomColorString());
    });


});

