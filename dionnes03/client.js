const net = require('net');
const readline = require('readline');

class MeuRPClient {
  constructor() {
    this.client = new net.Socket();
    this.PORT = 8080;
    this.client.connect(this.PORT, 'localhost', () => {
      console.log('Conectado ao servidor');
      this.keyboardInput();
    });

    this.client.on('close', () => {
      console.log('Conexão fechada');
    });
    this.functions = {};
  }

  //Para usar as funcs pelo terminal
  keyboardInput() {
    const rl = readline.createInterface({
      input: process.stdin,
      output: process.stdout,
      prompt: '>:',
    });

    rl.prompt();

    rl.on('line', (input) => {
      this.handleUserInput(input);
      rl.prompt();
    });
  }

  handleUserInput(input) {
    const parts = input.trim().split(' ');
    const command = parts[0];
    const params = parts.slice(1);

    if (command === 'call') {
      this.call(params[0], ...params.slice(1))
        .then((result) => {
          console.log(`Resposta: ${result}`);
        })
        .catch((error) => {
          console.error(`Erro: ${error.message}`);
        });
    } else {
      this.call(command, ...params)
        .then((result) => {
          console.log(`Resposta: ${result}`);
        })
        .catch((error) => {
          console.error(`Erro: ${error.message}`);
        });
    }
  }

  //Registra as funções em this.functions
  async registerFunctions() {
      try {
        const listFunctions = await proxy.call('listFunctions');
        
        if (Array.isArray(listFunctions)) {
          listFunctions.forEach((funcName) => {
            this.functions[funcName] = (...params) => this.call(funcName, ...params);
          });
          console.log('Funções registradas:', Object.keys(this.functions));
        } else {
          console.error('Resposta invalida do servidor:', listFunctions);
        }
      } catch (error) {
        console.error('JSON invalido do servidor');
      }

    this.client.once('error', (error) => {
      console.error(error);
    });
  }

  call(method, ...params) {
    return new Promise((resolve, reject) => {
      const request = JSON.stringify({ method, params });
      this.client.write(request);
      this.client.once('data', (data) => {
        try {
          const response = JSON.parse(data.toString());
          if (response.result !== undefined) {
            resolve(response.result);
          } else if (response.error) {
            reject(new Error(response.error));
          } else {
            reject(new Error('Resposta invalida do servidor'));
          }
        } catch (error) {
          reject(new Error('JSON invalido do servidor'));
        }
      });

      this.client.once('error', (error) => {
        reject(error);
      });
    });
  }

  close() {
    this.client.end();
  }
}

const proxy = new MeuRPClient();
proxy.registerFunctions();

async function main() {
  try {
    //É possível chamar assim
    const isEven = await proxy.call('is_even', 170);
    console.log(`170 is even: ${isEven}`);

    //Mas também é possível chamar assim pois as funções foram registradas pelo registerFunctions
    const isEven2 = await proxy.functions.is_even(169);
    console.log(`169 is even: ${isEven2}`);
 
    const soma = await proxy.functions.add(3, 2);
    console.log(`3 + 2 = ${soma}`);

    const sub = await proxy.functions.sub(7, 5);
    console.log(`7 - 5 = ${sub}`);

  } catch (error) {
    console.error(`Erro: ${error.message}`);
  } finally {
    // proxy.close();
  }
}

main();