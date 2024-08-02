function doGet(e) {
  ContentService.createTextOutput('I think,');
  var uidlog = e.parameter.uidlog;        //recebe a str do esp
  var esp32str = uidlog.replace(/_/g, ' ');     //troca as _ da string por vazio
  esp32str= '"' + esp32str + '"';
  
  var currentDate = new Date();
  var data = Utilities.formatDate(currentDate, Session.getScriptTimeZone(), 'yyyy-MM-dd');
  var hora = Utilities.formatDate(currentDate, Session.getScriptTimeZone(), 'HH:mm:ss');
  var uid;
  var quem;
  var status;

  if ( esp32str === '"BOTAO ATIVO"') {
    uid = "Botão_press";
    quem = "Interno";
    status = "Permitido";

  } else {
    var valueInThirdColumn = isStringInFirstColumn(esp32str);
    uid = uidlog
    
    if (valueInThirdColumn !== false) {
      quem = valueInThirdColumn;
      status = "Permitido";

    } else {
      quem = "Desconhecido";
      status = "Negado";
    }
  }

  updateSheet( ['A', 'B', 'C', 'D', 'E'], [data, hora, uid, quem, status], 'T', 12 )
  updateSheet( ['G', 'H', 'I', 'J', 'K'], [data, hora, uid, quem, status], 'U', 102 )
  updateSheet( ['M', 'N', 'O', 'P', 'Q'], [data, hora, uid, quem, status], 'V', 1000 )
  return ContentService.createTextOutput('therefore I am...');
}

function isStringInFirstColumn(searchString) {
  var sheet = SpreadsheetApp.openById('').getSheets()[0]; // Substitua 'YOUR_SPREADSHEET_ID' pelo ID real da sua planilha
  var data = sheet.getRange('A2:C').getValues();  // Obtém valores da coluna A à C
  
  for (var i = 0; i < data.length; i++) {
    if (data[i][0] === '') {
      // Retorna false se encontrar uma célula vazia
      return false;
    } else if (data[i][0] === searchString) {
      // Retorna o valor na terceira coluna se encontrar a string
      return data[i][2];
    }
  }
  
  // Retorna false se não encontrar a string
  return false;
}

function updateSheet(columnList, valueList, columnIndicator, limit) {
  // Acesse a planilha pelo ID
  var sheet = SpreadsheetApp.openById('').getActiveSheet();
  
  // Obtenha o valor da terceira linha da coluna indicada
  var currentRow = sheet.getRange(3, getColumnNumber(columnIndicator)).getValue();
  
  // Escreva os valores nas colunas especificadas na linha atual
  for (var i = 0; i < 5; i++) {
    sheet.getRange(currentRow, letterToNumber(columnList[i])).setValue(valueList[i]);
  }
  
  // Some +1 ao valor atual da terceira linha da coluna indicada
  var newValue = currentRow + 1;
  if (newValue > limit) {
    newValue = 3;
  }
  
  // Atualize o valor na terceira linha da coluna indicada
  sheet.getRange(3, getColumnNumber(columnIndicator)).setValue(newValue);

  function letterToNumber(letter) {
    return letter.charCodeAt(0) - 'A'.charCodeAt(0) + 1;
  }
}

function getColumnNumber(columnLetter) {
  // Converte a letra da coluna em um número
  var base = 'A'.charCodeAt(0);
  var column = columnLetter.charCodeAt(0);
  return column - base + 1;
}
