FROM node:18-alpine

WORKDIR /app

# Copia package.json e package-lock.json
COPY package*.json ./

# Installa TUTTE le dipendenze, incluse devDependencies
RUN npm install

# Copia il resto del progetto
COPY . .

# Comando di default
CMD ["npm", "run", "dev"]
