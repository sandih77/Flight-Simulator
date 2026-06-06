#!/bin/bash

run() {
    # 1. Nettoyage de l'ancien exécutable et des fichiers temporaires du MOC
    rm -f flight_simulator
    rm -f moc_*.cpp

    # 2. ÉTAPE DU MOC (Indispensable pour le Q_OBJECT)
    # On cherche tous les fichiers .hpp qui contiennent "Q_OBJECT" et on génère leur fichier moc_*.cpp
    echo "-- GÉNÉRATION DU MOC QT --"
    for hpp in $(find . -type f -name "*.hpp"); do
        if grep -q "Q_OBJECT" "$hpp"; then
            filename=$(basename "$hpp" .hpp)
            dirname=$(dirname "$hpp")
            # Appel du compilateur de meta-objets de Qt6
            /usr/lib/qt6/libexec/moc "$hpp" -o "$dirname/moc_$filename.cpp"
        fi
    done

    # 3. Récupération de tous les fichiers .cpp (incluant les nouveaux moc_*.cpp)
    find . -type f -name "*.cpp" > source.txt
    
    echo "-- COMPILATION --"
    
    # 4. Compilation g++ avec les drapeaux Qt6 injectés par pkg-config
    # Note : `pkg-config --cflags --libs Qt6Widgets` récupère automatiquement 
    # les dossiers -I include et les bibliothèques -lQt6Widgets nécessaires.
    g++ -I includes \
        $(pkg-config --cflags Qt6Widgets) \
        @source.txt \
        -o flight_simulator \
        $(pkg-config --libs Qt6Widgets)

    # 5. Nettoyage des fichiers textes et des mocs intermédiaires
    rm -f source.txt
    rm -f src/*/moc_*.cpp
    rm -f moc_*.cpp

    # 6. Lancement de l'application si la compilation a réussi
    if [ -f flight_simulator ]; then
        echo "-- RUNNING --"
        ./flight_simulator
    else
        echo "Échec de la compilation."
    fi
}

run