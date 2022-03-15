var show = false;

$(document).ready(function() {
    $("#btn").click(function () {
        if (show == true) {
            $("#art").hide();
            show = false;
            console.log(show);
            $("#btn").html("Click for documentation");
        }
        else {
            $("#art").show();
            show = true;
            $("#btn").html("Hide");

        }
        
    })
})