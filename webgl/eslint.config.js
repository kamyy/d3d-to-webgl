import js from "@eslint/js"
import globals from "globals"
import reactHooks from "eslint-plugin-react-hooks"
import reactRefresh from "eslint-plugin-react-refresh"
import tseslint from "typescript-eslint"
import prettier from "eslint-plugin-prettier/recommended"

export default tseslint.config(
  // Skip generated folders and installed dependencies.
  { ignores: ["dist", "node_modules"] },
  {
    // Start with the standard JavaScript, TypeScript, and Prettier rules.
    extends: [js.configs.recommended, ...tseslint.configs.recommended, prettier],
    files: ["**/*.{ts,tsx}"],
    languageOptions: {
      // Match the JavaScript version that TypeScript compiles this project to.
      ecmaVersion: 2022,
      globals: {
        // Browser names like window, document, and WebGLRenderingContext.
        ...globals.browser,
        // Test names like describe, it, and expect.
        ...globals.vitest,
      },
    },
    rules: {
      // Allow any when working with loose data or APIs that are hard to type.
      "@typescript-eslint/no-explicit-any": "off",
    },
  },
  {
    // React-specific checks only run on TSX files, not plain WebGL/utility code.
    files: ["**/*.tsx"],
    plugins: {
      "react-hooks": reactHooks,
      "react-refresh": reactRefresh,
    },
    rules: {
      // Catch common React Hook mistakes, like calling hooks conditionally.
      ...reactHooks.configs.recommended.rules,
      // Warn when a component file exports non-components, which can break fast refresh.
      "react-refresh/only-export-components": ["warn", { allowConstantExport: true }],
    },
  }
)
