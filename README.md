# DNACoder

DNACoder é um projeto acadêmico em C++ focado em processamento de sequências biológicas (DNA) com ênfase em **alto desempenho (HPC)**, **paralelismo** e **arquitetura modular**. O backend expõe funcionalidades via **gRPC** e utiliza **PostgreSQL** como armazenamento local (via TCP ou Unix socket).

> Objetivo: oferecer uma base sólida (reprodutível e extensível) para experimentos e pipelines de análise/transformação de sequências (ex.: codificação, estatísticas, classificação, etc.), mantendo rastreabilidade e qualidade de engenharia.

---

## Visão geral da arquitetura

- **API (gRPC)**: expõe operações (ex.: análise e streaming de resultados).
- **Infra (PostgreSQL / libpqxx)**: acesso ao banco `dnacoder`.
- **Engine (TBB / paralelismo)**: processamento intensivo de CPU (ex.: GC-content, checksum, codificação 2-bit).
- **Cliente gRPC (C++)**: ferramenta simples para testar o servidor.

Fluxo:
1. Cliente gRPC → 2. Servidor gRPC (DNACoder) → 3. PostgreSQL → 4. Engine paralela → 5. Resposta via stream gRPC.

---

## Stack técnica

- **C++20**
- **gRPC + Protobuf**
- **PostgreSQL** (local)
- **libpqxx** (cliente C++ para PostgreSQL)
- **oneTBB** (paralelismo e performance)
- **CMake + Conan** (build e dependências)
- **VS Code** (IDE recomendada)

---

## Pré-requisitos (Linux)

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build git python3-pip pipx
pipx ensurepath
# reabra o terminal
pipx install conan
