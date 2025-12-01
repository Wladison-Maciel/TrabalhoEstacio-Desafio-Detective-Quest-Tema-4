# 🕵️‍♂️ Detective Quest — Sistema Completo de Exploração, Pistas e Suspeitos

Este projeto implementa a fase final do jogo **Detective Quest**, desenvolvido pela *Enigma Studios*.  
O jogador explora uma mansão, coleta pistas, relaciona-as a suspeitos e faz uma acusação final — validada automaticamente pelo sistema.

---

## 🎮 Objetivo do Projeto

Criar um sistema completo em C que utiliza:

- **Árvore Binária** para navegação pelos cômodos.  
- **Árvore BST** para armazenar as pistas coletadas.  
- **Hash Table** para associar pistas a suspeitos.  
- **Interação em tempo real** com o jogador para exploração e julgamento.

---

## 🚧 Funcionalidades Implementadas

### ✔️ 1. Mapa da Mansão (Árvore Binária)
- Cada cômodo é um nó da árvore.
- O jogador inicia no **Hall de Entrada**.
- Pode escolher:  
  - Esquerda (**e**)  
  - Direita (**d**)  
  - Sair (**s**)

### ✔️ 2. Pistas Associadas
- Cada sala possui **uma pista estática** definida no código.
- Ao entrar no cômodo, a pista é exibida.

### ✔️ 3. Coleta de Pistas (BST)
- As pistas encontradas são inseridas em uma **árvore de busca binária**.
- Ao final, são exibidas **em ordem alfabética**.

### ✔️ 4. Tabela Hash de Suspeitos
- Cada pista é uma **chave**.
- O suspeito correspondente é o **valor**.
- Permite verificar rapidamente quem é relacionado a cada pista.

### ✔️ 5. Julgamento Final
- O jogador acusa um suspeito.
- O sistema verifica se **pelo menos duas pistas** apontam para ele.
- Resultado possível:
  - 🟩 *Acusação confirmada!*
  - 🟥 *Acusação inválida!*

---

## 🧩 Estruturas Utilizadas

### Structs
- `Sala` → nome + ponteiros da árvore.  
- `PistaNode` → nós da árvore BST.  
- `HashTable` → pares pista/suspeito.  

### Estruturas de Dados
- Árvore Binária (salas)  
- BST (pistas)  
- Hash Table (suspeitos)

---

## 🧠 Conceitos Trabalhados

- Árvores binárias  
- Árvores de busca  
- Hash tables  
- Structs e ponteiros  
- Alocação dinâmica  
- Recursividade  
- Modularização  
- Estruturas condicionais e loops

---

## 🛠️ Principais Funções

| Função | Descrição |
|--------|-----------|
| `criarSala()` | Cria dinamicamente um cômodo da mansão |
| `explorarSalas()` | Navega entre cômodos e coleta pistas |
| `inserirPista()` | Insere pistas na BST |
| `inserirNaHash()` | Associa pista → suspeito |
| `encontrarSuspeito()` | Consulta suspeito de uma pista |
| `verificarSuspeitoFinal()` | Executa o julgamento final |

---

## ▶️ Execução

Compile com:

```bash
gcc detective_quest.c -o detective
```
```bash
./detective
```