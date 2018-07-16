#include <SPI.h>
#include <Ethernet.h>

byte mac[]={0xDE,0xAD,0xBE,0xEF,0xFE,0xED};
IPAddress ip(192,168,1,200);
EthernetServer servidor(80);

String url;
//byte pageToShow = 1; // posso usar valores mesmo ao inves dessa variavel, pra economizar caso preciso
//int buzzer = 3;

//void showPage(int x);
//void loginPage();
void mainPage();
void monitoringPage();
void alarmActivated();

void setup(){
  //pinMode(buzzer, OUTPUT);

  //Inicializamos o servidor
  Ethernet.begin(mac, ip);
  servidor.begin();
  
  Serial.begin(9600);
}

void loop(){
  //showPage(1); // 1: loginPage, 2: main page, 3: alarm off page, 4: alarm activated
  //Serial.println("loop, depois de chamar função");
  loginPage();
}

/*void showPage(int x){
  switch (x){
    case 1:
      loginPage();
      //pageToShow = 2;
      break;
    case 2:
      //pageToShow = mainPage();
      break;
    case 3:
      //pageToShow = monitoringPage();
      break;
    //case 4: // teoricamente esse case não exixte pois essa função só vai ser chamada por interrupção, mas ta aqui por motivos de testes
      //alarmActivated();
  }
}*/

// aqui deve exibir opções: caixa de texto de login, e senha, e botão submit. então é verificado o dado, se correto
// mostra página com opção: ativar alarme (até aqui ok com bugs). se errado mostra aviso e deixa na mesma página para
// digitar de novo
void loginPage(){
  while(true){
    EthernetClient cliente = servidor.available();
    if(cliente){
      while(cliente.connected()){
        if(cliente.available()){
          
          char c = cliente.read();
          if(url.length() < 100)
            url += c;
          
          if(c == '\n'){
            cliente.println("HTTP/1.1 200 OK");
            cliente.println("Content-Type: text/html");
            cliente.println();
            cliente.print("<!DOCTYPE html>");
            cliente.print("<html lang='pt-br'>");
            
              cliente.println("<head>");
              cliente.println("<title>Login</title>");
              cliente.println("<style type='text/css'> body { color: black; } </style>");
              cliente.println("<meta charset='utf-8'>");
              //cliente.println("<meta http-equiv = 'refresh' content='3'; URL = 'http://192.168.1.200/'>");
              cliente.println("</head>");
              
              cliente.println("<body>");
            
                cliente.print("<center>");
                cliente.print("<h1>Página de login</h1>");
                
                cliente.print(" <div> ");
                  cliente.print(" <img src='https://cdn0.iconfinder.com/data/icons/user-icon-profile-businessman-finance-vector-illus/100/19-1User-512.png' align='center' height='200' width='200' > ");
                cliente.print("</div>");
                  cliente.print("<form method=\"get\">");
                    // caixas de texto
                    cliente.print("<input type='text' name='email' size='40' value='E-mail'/></br></br>");
                    cliente.print("<input type='text' name='senha' size='40' value='Senha'/></br><br/>");
                    
                    cliente.print("<button type='submit'>Envia para o Arduino</button>");
                  cliente.print("</form>");
                  
                  // não deu pra deixar la em cima porque não tem espaço no arduino
                  
                  if((url.indexOf('/?email=danielelielgaio%40gmail.com') >= 0) && (url.indexOf('1234') >= 0)) { // verificação dos dados de acesso
                    // se dados foram verificados mostra página home
                    //Serial.println("Dados de acesso condirmados");
                    url = "";
                    mainPage();
                    //return;
                  }//else{
                    //Serial.println("Dados de acesso incorretos");
                  //}
  
                cliente.print("</center>");
              cliente.println("</body>");
            cliente.println("</html>");
            
            delay(1);
            cliente.stop();
            url = "";
          }// fim if(c == \n)
        }//fim if(cliente.available())
      }//fim while
    }// fim if(cliente)
  }
}

void mainPage(){
  while(true){
    EthernetClient cliente = servidor.available();
    if(cliente){
      while(cliente.connected()){
        if(cliente.available()){
          char c = cliente.read();
          if(url.length() < 100)
            url += c;
          
          if(c == '\n'){
            cliente.println("HTTP/1.1 200 OK");
            cliente.println("Content-Type: text/html");
            cliente.println();
            cliente.print("<!DOCTYPE html>");
            cliente.print("<html lang='pt-br'>");
            
              cliente.println("<head>");
              cliente.println("<title>Home</title>");
              cliente.println("<style type='text/css'> body { color: black; } </style>");
              cliente.println("<meta charset='utf-8'>");
              //cliente.println("<meta http-equiv = 'refresh' content='3'; URL = 'http://192.168.1.200/'>");
              cliente.println("</head>");
              
              cliente.println("<body>");
            
                cliente.print("<center>");   
                cliente.print("<h1>Página principal</h1>");
                
                cliente.print(" <div> ");
                  cliente.print(" <img src='https://image.flaticon.com/icons/svg/2/2144.svg' align='center' height='200' width='200' > ");
                cliente.print("</div>");
                
                  cliente.print("<form method=\"get\">");
                    cliente.print("</br>");
                    cliente.print("<fieldset>");
                    
                      cliente.print("<legend>Menu</legend>");
                      cliente.print("<input type='radio' name = 'on'>Ativar alarme</br></br>");
                      //cliente.print("<input type='radio' name = 'off'>Desativar alarme</br></br>"); // opção desnecessária nesta página
                      //cliente.print("<input type='radio' name = 'on'>Visualizar cameras (Em breve!)</br></br>");
                      cliente.print("<button type='submit'>Envia para o Arduino</button>");
                      
                    cliente.print("</fieldset>");
                  cliente.print("</form>");
                  
                  // fazer verificação aqui, se clicar em ativar alarme, ir para pagina "monitorando"
                  if(url.indexOf("on=on") >= 0){
                    //Serial.println("Ligar alarme");
                    //pageToShow = 3;
                    //showPage(pageToShow);
                    monitoringPage();
                  }else if(url.indexOf("off=on") >= 0){
                    //Serial.println("Desligar alarm");
                  }
  
                cliente.print("</center>");
              cliente.println("</body>");
            cliente.println("</html>");
            
            delay(1);
            cliente.stop();
            url = "";
          }// fim if(c == \n)
        }//fim if(cliente.available())
      }//fim while
    }// fim if(cliente)
  }
}

void monitoringPage(){
  do{
    
    EthernetClient cliente = servidor.available();
   
    if(cliente){
      while(cliente.connected()){
        if(cliente.available()){
  
          char c = cliente.read();
          if(url.length() < 100)
            url += c;
          
          if(c == '\n'){ // \n indica que a requisição http terminou
            cliente.println("HTTP/1.1 200 OK");
            cliente.println("Content-Type: text/html");
            cliente.println();
            cliente.println("<!DOCTYPE html>");
            cliente.println("<html lang='pt-br'>");
            
              cliente.println("<head>");
                cliente.print("<title>Sistema de monitoramento</title>");
                cliente.print("<style type = 'text/css'> body { color: black; } </style>");
                cliente.print("<meta charset = 'utf-8'>");
                cliente.print("<meta http-equiv = 'refresh' content = '3'; URL = 'http://192.168.1.200/'>");
              cliente.print("</head>");
              
              cliente.print("<body>");
                cliente.print("<center>");
                
                  cliente.print("<h1>Monitorando movimentos</h1>");
                  
                  cliente.print("<div>");
                    cliente.print(" <img src='http://24.media.tumblr.com/aa14885e7d0d6661e0fb6ad9d5161e59/tumblr_n37a9zwwKq1sfvpsto1_500.gif' align='center' > ");
                  cliente.print("</div>");
                  
                  cliente.print("<form method=\"get\">");
                    cliente.print("</br>");
                    cliente.print("<fieldset>");
                    
                      cliente.print("<legend>Menu</legend>");
                      cliente.print("<input type='radio' name = 'off'>Desativar alarme</br></br>"); // opção desnecessária nesta página
                      cliente.print("<button type='submit'>Envia para o Arduino</button>");
                      
                    cliente.print("</fieldset>");
                  cliente.print("</form>");
                
                  if(url.indexOf("off=on") >= 0){
                    //Serial.println("Desligar alarm");
                    //pageToShow = 2;
                    //showPage(pageToShow);
                    mainPage();
                  }
                  
                cliente.print("</center>");
              cliente.println("</body>");
            cliente.println("</html>");
            
            delay(1);
            cliente.stop();
            url = "";
          }// fim if(c == \n)
        }//fim if(cliente.available())
      }//fim while
    }// fim if(cliente)
  }while(url.indexOf("off=on") >= 0);
}

// essa página é exibida quando o alarme é disparado
void alarmActivated(){
  while(true){
    //digitalWrite(buzzer, HIGH); // dispara o alarme quando a interrupção acontece
    EthernetClient cliente = servidor.available();
    
    if(cliente){
      while(cliente.connected()){
        if(cliente.available()){
  
          char c = cliente.read();
          if(url.length() < 100)
            url += c;
          
          if(c == '\n'){
            cliente.println("HTTP/1.1 200 OK");
            cliente.println("Content-Type: text/html");
            cliente.println();
            cliente.print("<!DOCTYPE html>");
            cliente.print("<html lang='pt-br'>");
            
              cliente.println("<head>");
              cliente.println("<style type='text/css'> body { color: black; } </style>");
              cliente.println("<meta charset='utf-8'>");
              //cliente.println("<meta http-equiv = 'refresh' content='3'; URL = 'http://192.168.1.200/'>"); // usar se precisar
              cliente.println("<title>ATENÇÃO</title>");
              cliente.println("</head>");
              
              cliente.println("<body>");
                cliente.print("<center>");
                
                  cliente.print("<h1>O alarme foi disparado!</h1>");
                  
                  cliente.print("<script type = text/javascript>");
                    cliente.print("alert('O alarme foi acionado, recomendamos ferivicar o local!');");
                  cliente.print("</script>");
                  
                  cliente.print(" <div> ");
                    cliente.print(" <img src='http://www.gif-maniac.com/gifs/53/53443.gif' align='center' height='200' width='400' > ");
                  cliente.print("</div>");
                  
                  cliente.print("<form method=\"get\">");
                    cliente.print("</br>");
                    cliente.print("<fieldset>");
                    
                      cliente.print("<legend>Menu:</legend>");
                      cliente.print("<input type='radio' name = 'desligarAlarm'>Desligar alarme</br></br>");
                      //cliente.print("<input type='radio' name = 'seeCameras'>Visualizar cameras (Em breve!)</br></br>");
                      cliente.print("<button type='submit'>Envia para o Arduino</button>");
                      
                    cliente.print("</fieldset>");
                  cliente.print("</form>");
                  
                  cliente.print("<a href ='https://www.delegaciaonline.rs.gov.br/dol/#!/registro/Violação%20de%20domicílio'>FAZER UM REGISTRO DE OCORRÊNCIA</a>");
                  
                  if(url.indexOf("desligarAlarm") >= 0){
                    Serial.println("alarme foi desligado");
                    //pageToShow = 2;
                    //showPage(pageToShow);
                    mainPage();
                  }else if(url.indexOf("seeCameras") >= 0){
                    Serial.println("vendo cameras");
                  }
                  
                cliente.print("</center>");
              cliente.println("</body>");
            cliente.println("</html>");
            
            delay(1);
            cliente.stop();
            url = "";
          }// fim if(c == \n)
        }//fim if(cliente.available())
      }//fim while
    }// fim if(cliente)
  }
}