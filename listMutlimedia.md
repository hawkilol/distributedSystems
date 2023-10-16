1. Responda se as afirmativas abaixo são verdadeiras ou falsas, justificando sua resposta em ambos
os casos.
a) A comutação de pacotes é sempre melhor que a comutação de circuitos para qualquer tipo de
aplicação multimídia.
Falso, pois em aplicações sensiveis ao tempo como uma chamada de voz se beneficiam da alocação do canal fisico da comutação de circuitos.

b) A digitalização de sinais analógicos é obtida com três processos: amostragem, quantização e
codificação.
Verdade, amostragem captura momentos de um sinal analogico de acordo com uma taxa (intervalo) de amostragem, assim os valores
obtidos pela amostragem são arredondados para valores discretos, assim na codificação esses valores são representados em uma saida
em binario 

c) A codificação espacial de vídeo é realizada entre imagens consecutivas.
Falso, a codificação espacial se refere a compressão de um unico quadro, é a codificação temporal que interpola a diferença entre quadros.

d) O protocolo SIP é utilizado para garantir qualidade de serviço para aplicações de vídeo em
redes baseada em comutação de pacotes.
Falso, o SIP ajuda a estabelecer uma Sessão e ele não aloca recursos para prover garantias nessa sessão.

3. O que são Redes Multimídia?
   Redes Multimídia são redes que sustentam aplicações que trabalham como tipos de dados dinamicos como voz e video, as quais precisam de uma qualidade
   de serviço minima pois podem ser tempo real e sensiveis a atrasos.
 Quais são as principais características e requisitos de aplicações multimídia? Explique cada
um deles.
O requisito principal das redes mutlimidia é a Qualidade de serviço a qual possui seus proprios requisitos pois pode ser a feta por:
largura de banda, para aplicações como o streaming altas taxas de transmissão são necessárias, além disso é bom ter banda sobrando
para permitir que varios usuario usem a mesma rede sem congestiona ela.
Atraso, aplicações voip por exemplo só são uteis em tempo real pois se o atraso for muito grande não é possivel conversar de forma fuilda
como na vida real com uma pessoal assim aplicações sensiveis ao tempo tem que ter uma atraso minimo.
Jitter, é o atraso entre pacotes, o que pode causar atrasos para buffers de streaming ou bugs para aplicações sensiveis ao tempo pois se o
o Jitter não for tratado ou seja esses pacotes são reproduzidos assim que chegam uma chamada não vai ser fluida, logo, assim para manter a
integridade do conteudo é preciso sincronizar os pacotes o que pode causar atrasos gerais.
Interatividade,
Tolerancia a perdas, é preciso garantir que perdas não o ocorram ou possui mecanismos que tratem o erro ou sejam tolerantes ao erro
atraves de redundancia.
 
6. Quais são as principais fontes de atraso em redes baseadas em comutação de pacotes?
Descreva cada uma delas.
Perdas, muitas vezes atrasos ocorrem porque um pacote foi perdido e teve que ser reenviado
Atraso de propagação, as vezes a qualidade do meio fisico de alguns enlaçes da rede não é de boa qualidade em comparação com as bordas
Atraso na Fila, os pacotes podem perder tempo esperado na fila de roteadores por diversos motivos um deles é o head of line blocking
no qual um pacote na frente da fila não pode ser enviado por algum motivo como controle de congestionamento, e os pacote atras na fila
teoricamente estão ok para a transmissão mas o pacote na frente não deixa.
Atraso de processmento e roteamento, os pacotes devem ser reconhecidos e enviados para seu destino mas isso depende do tempo de processmento do pacote.
Atraso de transmissão, depende a capacacidade do enlaçe de dar vasão ao pacote, assim se um pacote for muito gordo para uma baixa largura
de sinal ele vai ter atraso na transmissão
8. Cite e explique três soluções desenvolvidas para estender a Internet tradicional para suportar
aplicações multimídia.
    Qualidade de Serviço (QoS): A QoS é uma abordagem que se concentra na priorização do tráfego de rede para atender às necessidades de aplicações multimídia sensíveis a atrasos, como vídeo e voz sobre IP. A QoS envolve vários mecanismos e protocolos para garantir que os pacotes de dados sejam tratados de maneira diferenciada com base em suas necessidades. Alguns componentes da QoS incluem:

        Marcação de Pacotes (DiffServ e IP precedence): Os pacotes são marcados com níveis de prioridade, permitindo que os dispositivos de rede priorizem a entrega de acordo com essas marcações.

        Gerenciamento de Largura de Banda: A largura de banda é alocada de maneira justa e eficiente para garantir que as aplicações multimídia recebam os recursos de rede de que precisam.

        Controle de Congestionamento: Mecanismos de controle de congestionamento, como o TCP (Transmission Control Protocol), ajudam a evitar congestionamentos na rede, o que pode prejudicar a qualidade das aplicações multimídia.

    Protocolos Multimídia: Protocolos dedicados foram desenvolvidos para lidar com aplicações multimídia. Alguns exemplos notáveis incluem:

        Real-Time Transport Protocol (RTP): O RTP é um protocolo que transporta áudio e vídeo em tempo real pela rede. Ele lida com a sincronização de mídia, a detecção de perda de pacotes e a compensação de atrasos.

        Real-Time Control Protocol (RTCP): O RTCP é usado em conjunto com o RTP para monitorar a qualidade e o desempenho da transmissão em tempo real, fornecendo informações de feedback aos aplicativos multimídia.

        Session Initiation Protocol (SIP): O SIP é um protocolo de sinalização usado para estabelecer e gerenciar sessões multimídia, como videochamadas e conferências.

    Redes de Entrega de Conteúdo (CDN - Content Delivery Networks): As CDNs são uma solução para melhorar a distribuição de conteúdo multimídia na Internet. Elas consistem em servidores distribuídos geograficamente que armazenam cópias de conteúdo (como vídeos) e os entregam aos usuários a partir de servidores próximos a eles. Isso reduz a latência e a carga nos servidores de origem, melhorando a experiência do usuário.

    As CDNs ajudam a reduzir o atraso de propagação, uma das principais fontes de atraso em redes de comutação de pacotes, ao colocar o conteúdo mais perto dos usuários. Além disso, elas podem realizar tarefas de otimização, como a compressão de vídeo, para economizar largura de banda.




10. Explique como funciona o mecanismo de escalonamento Weighted Fair Queueing (WFQ).
  É um algoritmo que procura particionar a banda de forma justa, diferente do Round robin ele não trata a banda de forma uniforme, uma vez que
  o RR procura ser equilitario ou seja todos tem o mesmo tempo, ou seja, as mesmas oportunidades para acessar recursos equanto o WFQ
  procura balancer os tempos de acordo om o seu peso, então uma fila vazia tem o um peso menor ou seja é mais justo priorizar outras, mas
  assim a banda das filas vai ser proporcional a ao peso logo em media todos as filas vão ter proporcionalmente a mesma banda.
12. Explique como funciona o mecanismo de escalonamento Weighted Fair Queueing (WFQ).
13. Explique o funcionamento do balde de fichas.
    O balde de fichas é um mecanismo de controle de taxa de transmissão para evitar que uma rede fique sobrecarregada,
    O balde de fichas virtual recebe fichas é um pacote na fila se quiser ser transmitido precisa gastar uma ficha do balde para isso,
    logo ele não tiver fichas no balde ele tem que esperar uma ficha para poder ser transmitido,
    O balde furado permite uma taxa de transmissão fixa para pacotes que chegam no balde até mesmo em uma taxa variada, e o balde possui um
    tamanho limite que pode descartar pacotes.
15. Quais são os requisitos de comunicação das mídias voz e vídeo para que possam ser
transmitidas em redes multimídia? Descreva cada um deles.
16. Descreva para que serve o protocolo RTP.
    Ele oference recursos que podem melhor a qualidade de serviço de aplicações sensiveis a tempo, como uma padronização da estrutura do pacote,
    com identificação erros, informações relevantes sobre os pacotes como, tipo de codificação, timestamps para sincronização e número de sequencia
18. Descreva para que serve o protocolo RTCP.
    Ele serve como uma adição ao RTP, estando associado a ele em uma porta diferente, ele oference recursos como relatorios do remente e receptor, como fração de pacotes perdidos e jitter medio
