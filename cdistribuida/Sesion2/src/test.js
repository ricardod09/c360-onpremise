$( document ).ready(function() {

        $("#ejecutar").click( function(){

            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "http://127.0.0.1:5984/libros/_design/testlibros/_view/consultaautores?group=true&reduce=true", false);
            xhttp.setRequestHeader("Content-type", "application/json");
            xhttp.send();
            var response = JSON.parse(xhttp.responseText);

            $('#resultado').val(JSON.stringify(response,null, 4));

            //Llenar la tabla con los resultados
              var table = $("#tablaejemplo tbody");
              $.each(response.rows, function(idx, elem){
                  console.log(elem)
                  table.append('<tr><td>' + elem.key + '</td><td>' + elem.value +'</td></tr>');
              });

           }
        );
});
