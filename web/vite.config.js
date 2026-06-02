import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";
import { maridoApiPlugin } from "./server/api.js";

export default defineConfig({
  plugins: [react(), maridoApiPlugin()],
});
