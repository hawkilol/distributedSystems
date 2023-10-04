const net = require('net');
function functionsToSource(obj) {
  const result = {};
  for (const key in obj) {
    if (typeof obj[key] === 'function') {
      result[key] = obj[key].toString();
    } else {
      result[key] = obj[key];
    }
  }
  return result;
}

const functionMap = {
  is_even: (number) => number % 2 === 0,
  add: (a, b) => a + b,
  subtract: (a, b) => a - b,
  quickSort: (arrString) => quickSort(arrString),
  help: () =>  JSON.stringify(functionsToSource(functionMap), null, 2)
  // Add more functions here
};

const server = net.createServer((socket) => {
  console.log('Client connected');

  socket.on('data', (data) => {
    try {
      const request = JSON.parse(data.toString());
      console.log(request);
      if (request && request.method && functionMap[request.method]) {
        const result = functionMap[request.method](...request.params);
        socket.write(JSON.stringify({ result }));
      } else {
        socket.write(JSON.stringify({ error: 'Invalid RPC call' }));
      }
    } catch (error) {
      socket.write(JSON.stringify({ error: 'Invalid JSON' }));
    }
  });

  socket.on('end', () => {
    console.log('Client disconnected');
  });
});

const PORT = 8080;
server.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});


//Funcs
function partition(arr, low, high) {
	let pivot = arr[high];
	let i = low - 1;

	for (let j = low; j <= high - 1; j++) {
		// If current element is smaller than the pivot
		if (arr[j] < pivot) {
			// Increment index of smaller element
			i++;
			// Swap elements
			[arr[i], arr[j]] = [arr[j], arr[i]];
		}
	}
	// Swap pivot to its correct position
	[arr[i + 1], arr[high]] = [arr[high], arr[i + 1]];
	return i + 1; // Return the partition index
}

function recursiveQuickSort(arr, low, high) {
	if (low >= high) return;
	let pi = partition(arr, low, high);

	recursiveQuickSort(arr, low, pi - 1);
	recursiveQuickSort(arr, pi + 1, high);
}
function quickSort(arr){
  arr = arr.split(',').map(Number)
  recursiveQuickSort(arr, 0, arr.length - 1);
  return arr
}
