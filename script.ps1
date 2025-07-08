# Script para borrar carpetas y ejecutar un comando 'make run'

# --- Configuración ---
$Carpeta1 = "C:\Users\tito\Desktop\p2top3\-p"
$Carpeta2 = "C:\Users\tito\Desktop\p2top3\build"

$RutaProyecto = $PSScriptRoot
$ComandoMake = "make run"

# --- Borrar Carpetas ---
Write-Host "Intentando borrar carpeta: $Carpeta1"
If (Test-Path $Carpeta1) {
    try {
        Remove-Item -Path $Carpeta1 -Recurse -Force -ErrorAction Stop
        Write-Host "Carpeta ${Carpeta1} borrada correctamente." -ForegroundColor Green
    } catch {
        Write-Host "Error al borrar la carpeta ${Carpeta1}: $($_.Exception.Message)" -ForegroundColor Red
    }
} else {
    Write-Host "Carpeta ${Carpeta1} no existe. Saltando eliminación." -ForegroundColor Yellow
}

Write-Host "Intentando borrar carpeta: ${Carpeta2}"
If (Test-Path $Carpeta2) {
    try {
        Remove-Item -Path $Carpeta2 -Recurse -Force -ErrorAction Stop
        Write-Host "Carpeta ${Carpeta2} borrada correctamente." -ForegroundColor Green
    } catch {
        Write-Host "Error al borrar la carpeta ${Carpeta2}: $($_.Exception.Message)" -ForegroundColor Red
    }
} else {
    Write-Host "Carpeta ${Carpeta2} no existe. Saltando eliminación." -ForegroundColor Yellow
}

Clear-Host # Limpia la consola

# --- Ejecutar el comando "make run" ---
Write-Host "Intentando ejecutar el comando: '$ComandoMake' en el directorio: '$RutaProyecto'"

# # Verificamos si el directorio del proyecto (donde está el script) existe
If (Test-Path $RutaProyecto -PathType Container) {
    try {
        Start-Process -FilePath "cmd.exe" -ArgumentList "/c $ComandoMake" -WorkingDirectory $RutaProyecto -Wait -NoNewWindow
        Write-Host "Comando '$ComandoMake' ejecutado exitosamente en '$RutaProyecto'." -ForegroundColor Green
    }
    catch {
        Write-Host "Error al ejecutar el comando '${ComandoMake}': $($_.Exception.Message)" -ForegroundColor Red
    }
} else {
    Write-Host "El directorio del script '$RutaProyecto' no existe. No se ejecutará el comando '$ComandoMake'." -ForegroundColor Red
}

Write-Host "`nScript finalizado." -ForegroundColor Blue
