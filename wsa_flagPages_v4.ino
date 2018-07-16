//NOTES: esta havendo um alto consumo de memoria por variaveis globais, resolver isso!

// bibliotecas utilizadas
#include <Ethernet.h>
#include <Ultrasonic.h>

// zona 1 - sensor
#define TRIGGER_PIN_1 2
#define ECHO_PIN_1 4
Ultrasonic ultrasonic(TRIGGER_PIN_1, ECHO_PIN_1);

// zona 2 - sensor
#define TRIGGER_PIN_2 3
#define ECHO_PIN_2 5
Ultrasonic ultrasonic2(TRIGGER_PIN_2, ECHO_PIN_2);

// configurações da shiel ethernet
byte mac[]={0xDE,0xAD,0xBE,0xEF,0xFE,0xED};
IPAddress ip(192,168,1,200);
EthernetServer servidor(80);

// variáveis globais
String url;
//byte buzzer = 6, led = 7;
boolean loginPage=false, mainPage=false, monitoringPage=true, alarmActivated=false; // flags de controle da exibição das páginas

void setup(){
  //pinMode(buzzer, OUTPUT);
  //pinMode(led, OUTPUT);
  
  //Inicializamos o servidor
  Ethernet.begin(mac, ip);
  servidor.begin();
  
  Serial.begin(9600);
}

// funções extras
float lerSensor(int i){
  if(i == 1){
    float sensor_1;
    long microsec_1 = ultrasonic.timing();
    return sensor_1 = ultrasonic.convert(microsec_1, Ultrasonic::CM);
  }else{
    float sensor_2;
    long microsec_2 = ultrasonic2.timing();
    return sensor_2 = ultrasonic2.convert(microsec_2, Ultrasonic::CM);
  }
}

void loop(){
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
          cliente.println(); //esta linha em branco é nescessária ao cógigo
          cliente.print("<!DOCTYPE html>");
          cliente.print("<html lang='pt-br'>");
          
            cliente.println("<head>");
              //------------------------------------------- condicionantes para o titulo de página ----------------------------------
              if(loginPage){
                cliente.println("<title>Login</title>");
              }else if(mainPage){
                cliente.println("<title>Home</title>");
              }else if(monitoringPage){
                cliente.print("<title>Sistema de monitoramento</title>");
              }else if(alarmActivated){
                cliente.println("<title>ATENÇÃO</title>");
              }
              // --------------------------------------------------------------------------------------------------------------------
              cliente.println("<style type='text/css'> body { color: black; } </style>");
              cliente.println("<meta charset='utf-8'>");
              if(monitoringPage){ cliente.println("<meta http-equiv = 'refresh' content='3'; URL = 'http://192.168.1.200/'>"); }
            cliente.println("</head>");
            cliente.println("<body>");
              cliente.print("<center>");
              //------------------------------------------- condicionantes para o titulo do corpo -----------------------------------
              if(loginPage){
                cliente.print("<h1>Página de login</h1>");
              }else if(mainPage){
                cliente.print("<h1>Página principal</h1>");
              }else if(monitoringPage){
                cliente.print("<h1>Monitorando movimentos</h1>");
              }else if(alarmActivated){
                cliente.print("<h1>O alarme foi disparado!</h1>");
                cliente.print("<script type = text/javascript>");
                cliente.print("alert('O alarme foi acionado, recomendamos ferivicar o local!');");
                cliente.print("</script>");
              }
              // --------------------------------------------------------------------------------------------------------------------
              cliente.print(" <div> ");
                //------------------------------------------- condicionantes para a imagem ------------------------------------------
                if(loginPage){
                  cliente.print(" <img src='https://cdn0.iconfinder.com/data/icons/user-icon-profile-businessman-finance-vector-illus/100/19-1User-512.png' align='center' height='200' width='200' > ");
                }else if(mainPage){
                  cliente.print(" <img src='https://image.flaticon.com/icons/svg/2/2144.svg' align='center' height='200' width='200' > ");
                }else if(monitoringPage){
                  cliente.print(" <img src='http://24.media.tumblr.com/aa14885e7d0d6661e0fb6ad9d5161e59/tumblr_n37a9zwwKq1sfvpsto1_500.gif' align='center' > ");
                }else if(alarmActivated){
                  cliente.print(" <img src='http://www.gif-maniac.com/gifs/53/53443.gif' align='center' height='200' width='400' > ");
                }
                // ------------------------------------------------------------------------------------------------------------------
              cliente.print("</div>");
              cliente.print(" <div> ");
                //------------------------------------------- condicionantes para o corpo (conteudo) ----------------------------------
                if(loginPage){
                  cliente.print("<form method=\"get\">");
                    // caixas de texto
                    cliente.print("<input type='text' name='email' size='40' value='E-mail'/></br></br>");
                    cliente.print("<input type='text' name='senha' size='40' value='Senha'/></br><br/>");
                    cliente.print("<button type='submit'>Envia para o Arduino</button>");
                  cliente.print("</form>");
                  // verificação dos dados de acesso
                  if((url.indexOf('danielelielgaio@gmail.com') >= 0) && (url.indexOf('1234') >= 0)){
                    // se dados foram verificados mostra página home
                    //mainPage = true;
                    //loginPage = false;
                    // aqui tem que limpar todo o conteudo antigo da página
                    //cliente.print("document.body.innerHTML =\" \";"); // isso seria o que limpa a página html
                    // como ainda não sei apagar
                    //cliente.print("</br></br></br>");
                  }else
                    cliente.println("</br>Digite seus dados de acesso!");
                }else if(mainPage){
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
                  // se clicar em ativar alarme, ir para pagina "monitorando"
                  if(url.indexOf("on=on") >= 0){
                    //Serial.println("Ligar alarme");
                    //mainPage = false;
                    //monitoringPage = true;
                  }//else if(url.indexOf("off=on") >= 0){
                    //Serial.println("Desligar alarm");
                  //}
                }else if(monitoringPage){
                  // verifica intervalo de valor de leitura
                  if((lerSensor(1) >= 105) && (lerSensor(1) <= 125)){
                    //tudo ok
                  }else{
                    alarmActivated=true;
                    //loginPage=false;
                    //mainPage=false;
                    monitoringPage=false;
                    //digitalWrite(buzzer, HIGH);
                    //digitalWrite(led, HIGH);
                    Serial.println("Alarme da zona 1 foi ativado!");
                  }
                  if((lerSensor(2) >= 130) && (lerSensor(2) <= 150)){
                     //tudo ok
                  }else{
                     alarmActivated=true;
                     monitoringPage=false;
                     Serial.println("Alarme da zona 2 foi ativado!");
                  }
                  cliente.print("<form method=\"get\">");
                    cliente.print("</br>");
                    cliente.print("<fieldset>");
                    
                      cliente.print("<legend>Menu</legend>");
                      cliente.print("<input type='radio' name = 'off'>Desativar alarme</br></br>");
                      cliente.print("<button type='submit'>Envia para o Arduino</button>");
                      
                    cliente.print("</fieldset>");
                  cliente.print("</form>");
                  // verifica se selecionou desativar alarme
                  if(url.indexOf("off=on") >= 0){
                    //monitoringPage = false;
                    //mainPage = true;
                  }
                }else if(alarmActivated){
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
                  
                  if(url.indexOf('desligarAlarm') >= 0){
                    //Serial.println("alarme foi desligado");
                    url = "";
                    alarmActivated = false;
                    mainPage = true;
                  }else if(url.indexOf('seeCameras') >= 0){
                    //Serial.println("vendo cameras");
                  }
                }
                // --------------------------------------------------------------------------------------------------------------------
              cliente.print("</div>");
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