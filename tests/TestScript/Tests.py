import os
import subprocess

def parse_output(output):
  """
  Extrait les parties "output" et "erreur" du résultat de minicc.

  Args:
    output: La sortie complète de la commande minicc.

  Returns:
    Un tuple contenant (output, error).
  """
  error = ""
  output = ""
  for line in output.splitlines():
    if "error:" in line:
      error += line + "\n"
    elif "output:" in line:
      output += line + "\n"
  return output, error

def main():
  """
  Parcourt le dossier /path/ et exécute minicc sur chaque fichier .c.
  """
  path = "../Syntaxe/KO/"
  for filename in os.listdir(path):
    if filename.endswith(".c"):
      filepath = os.path.join(path, filename)
      print(f"Compiling {filename}...")
      # Exécution de minicc
      process = subprocess.run(
          ["../../minicc", filepath],
          stderr=subprocess.PIPE,
          universal_newlines=True,
      )
      
      # Extraction des parties "output" et "erreur"
      output, error = parse_output(process.stdout)
      # Affichage des résultats
      print(f"** Erreurs :**\n{process.stderr}")
      if output:
        print(f"** Sortie :**\n{output}")

if __name__ == "__main__":
  main()
