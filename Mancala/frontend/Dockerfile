# ==========================================
# Stage 1: Build Environment
# ==========================================
FROM node:20-alpine AS builder

WORKDIR /app

# Copy dependency configuration files
COPY package.json package-lock.json* ./

# Install packages
RUN npm install

# Copy full source tree and compile build
COPY . .
RUN npm run build

# ==========================================
# Stage 2: Minimal Web Server Runtime
# ==========================================
FROM nginx:alpine

# Copy compiled static assets from builder stage
COPY --from=builder /app/dist /usr/share/nginx/html

# Copy Nginx proxy configuration
COPY nginx.conf /etc/nginx/conf.d/default.conf

# Expose default HTTP port
EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]
