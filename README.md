# Projeto: Ordenação Multithread com Docker e Deploy Automatizado

## 💻 Sobre o Projeto

Este projeto tem como objetivo:
- Implementar a ordenação de arrays utilizando múltiplas threads com a biblioteca POSIX (`pthread`) em C.
- Comparar o desempenho entre execuções sequenciais e paralelas (com 1, 2 e 4 threads).
- Utilizar Docker para empacotar o projeto.
- Realizar deploy automatizado na AWS Lightsail usando GitHub Actions.

O projeto também mede o tempo de execução para avaliar o ganho de performance através da paralelização.

---

## ⚙️ Instruções de Compilação e Execução

### Dependências Necessárias:
- Sistema Linux ou WSL (Windows Subsystem for Linux).
- `gcc` instalado (compilador C).
- `pthread` library (normalmente já incluída no Linux).
- Docker instalado (para execução com containers).

### Para compilar manualmente:
```bash
gcc -pthread -o sorting_app main.c
```

### Para executar manualmente:
```bash
./sorting_app
```

### Para construir e correr com Docker:
```bash
docker pull frans203/multithread-sorting-app:latest
 sudo docker run --name sorting-app frans203/multithread-sorting-app:latest
```

### Para usar GitHub Actions:
- Configurar secrets no repositório:
  - `DOCKER_USERNAME`, `DOCKER_PASSWORD`, `LIGHTSAIL_HOST`, `LIGHTSAIL_USER`, `LIGHTSAIL_PRIVATE_KEY`.
- Push para a branch `master` aciona o workflow automático de build, push e deploy.

---

##  Instruções de Uso

- Muda o número de threads diretamente no código (variável `thread_counts`).
- O programa gera arrays aleatórios para testar cada execução.
- No final de cada teste, é impresso o tempo de execução em milissegundos para cada configuração de threads.
- Para comparar:
  - Corre o programa várias vezes e anota os tempos.
  - Cria uma tabela ou gráfico para análise visual.

---

## 📊 Comparativo de Desempenho

| Versão do Código         | Nº de Threads | Tempo de Execução (ms) |
|--------------------------|:-------------:|:------------------------:|
| Sequencial               | 1             | 0.001 ms                 |
| Código principal (main)  | 2             | 1.312 ms                 |
| Código com 4 threads     | 4             | 2.371 ms                 |

###  Observações:
- Com arrays pequenos, o ganho de performance com mais threads nem sempre é garantido.
- O código com 2 threads foi mais eficiente que o com 4 threads, pois a sobrecarga de criação e sincronização de múltiplas threads superou os benefícios do paralelismo.

---

##  Responsáveis pelo Projeto

### Desenvolvimento do Código Multithread:
- [Francisco Santana]

### Desenvolvimento do código sequencial:
- [Rainer Carneiro]

### Desenvolvimento do código de 4 threads:
- [João Vittor Alves/Rainer Carneiro]

### Implementação do Docker e Dockerfile:
- [Francisco Santana]

### Configuração do GitHub Actions para CI/CD:
- [Francisco Santana]

### Configuração e Administração AWS Lightsail:
- [Francisco Santana]

### Documentação e README:
- [João Vittor Alves]

---

