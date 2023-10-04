const net = require('net');
const readline = require('readline');

class RPCClient {
  constructor() {
    this.client = new net.Socket();
    this.PORT = 8080;
    this.client.connect(this.PORT, 'localhost', () => {
      console.log('Connected to server');
      this.initializeInput();
    });

    this.client.on('close', () => {
      console.log('Connection closed');
    });
  }

  initializeInput() {
    const rl = readline.createInterface({
      input: process.stdin,
      output: process.stdout,
      prompt: '> ',
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
      // Handle programmatic RPC call
      this.call(params[0], ...params.slice(1))
        .then((result) => {
          console.log(`Result: ${result}`);
        })
        .catch((error) => {
          console.error(`Error: ${error.message}`);
        });
    } else {
      // Send user input to the server for processing
      this.call(command, ...params)
        .then((result) => {
          console.log(`Result: ${result}`);
        })
        .catch((error) => {
          console.error(`Error: ${error.message}`);
        });
    }
  }

  call(method, ...params) {
    return new Promise((resolve, reject) => {
      const request = JSON.stringify({ method, params });
      console.log(request)
      this.client.write(request);

      this.client.once('data', (data) => {
        try {
          console.log(data.toString())
          const response = JSON.parse(data.toString());
          console.log(response)
          if (response.result !== undefined) {
            resolve(response.result);
          } else if (response.error) {
            reject(new Error(response.error));
          } else {
            reject(new Error('Invalid response from server'));
          }
        } catch (error) {
          reject(new Error('Invalid JSON response'));
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

const proxy = new RPCClient();

// Example usage
async function main() {
  try {
    const functionMap = {
      is_even: (number) => number % 2 === 0,
      add: (a, b) => a + b,
      subtract: (a, b) => a - b
      // Add more functions here
    };
    console.log(functionMap)
    const isEven = await proxy.call('is_even', 170);
    console.log(`170 is even: ${isEven}`);
    const sum = await proxy.call('add', 2, 3);
    console.log(`2 + 3 = ${sum}`);
    
    // Programmatic RPC call
    const difference = await proxy.call('subtract', 5, 3);
    console.log(`5 - 3 = ${difference}`);
  } catch (error) {
    console.error(`Error: ${error.message}`);
  } finally {
    // proxy.close();
  }
}

main();
