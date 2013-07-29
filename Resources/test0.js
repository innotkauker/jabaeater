function(){
    Responce.write("666".toInt());

    var i, j = 0;
    for(i = 0; i <= 5; i = i + 1){
        //var Environment;
        Responce.write("string " + i + " ", i, "   ");
        Responce.endl();
        //continue;
        if(!j){
            var j = 5;
            Responce.write(j);
            if(j == 6)
                break;
        }
        Responce.write(j);
    }
    Responce.endl();
    Responce.write("Environment.something() returned ", Environment.DATE());
    Responce.endl();

}
