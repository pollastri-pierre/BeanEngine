#pragma once

/*!
 *  @author Pierre Pollastri
 *  @version 0.4
 *  @file BApplication.h
 *  @brief Fichier de la classe Abstraite BApplication
 */

#include <iostream>
#include "BString.h"
#include <map>

#define B_PROFILES_DIR "profiles"
#define B_RESOURCES_DIR "resources"
#define B_SCRIPTS_DIR "scripts"

/*!
 *  @french
 *      @brief Classe abstraite de gestion du cycle de vie d'une application.
 *
 *      Classe de base de l'application. Cette classe n'est pas directement concernée par le moteur de jeux. Elle doit instancier le moteur de jeux,
 *      et doit lui lier les modules compatibles avec le système d'exploitation. Cette classe abstraite doit être reimplémentés pour chaque plateforme.
 *      C'est une classe singleton, accessible durant tout l'exécution de l'application
 *  @end
 *
 *  @english
 *      @brief An abstract class that manages the application life cycle.
 *
 *      This class is the base of an application. It is the bound between the operating system and the Bean Engine. It must instanciate all game engine modules and
 *      link them to the Bean engine. This class is one of the system dependent classes of the Bean Engine and must be rewrite for each operating system.
 *      This is a singleton class available every where while the application is running.
 *  @end
 */
class BApplication {

public:
    /*!
     *  @french
     *      @brief Une classe contenant le chemin d'accès d'un dossier pouvant être utilisé par l'application.
     *  
     *      Cette classe représente un dossier accessible dans l'application. Cette classe a été créé afin de gérer uniformément les
     *      accès aux dossiers sur les differentes plateformes.
     *  @end
     *  
     *  @english
     *      @brief This class holds the path to an available directory.
     *
     *      This class holds the path to an available directory. This class is designed to handle evenly the access to directories on the various
     *      operating systems
    *   @end
     */
	class Directory {
		BString _path; //! < @french Chemin d'accès au répertoire @end @english Directory path @end
		BString _dirName; //! < @french Nom du dossier @end @english Directory name @end
		bool 	_ok;
	public:

		Directory() {};
        /*!
         *  @french
         *      @brief Constructeur de la classe.
         *      
         *      Ce constructeur permet de définir sous quel nom  le repertoire sera accessible
         *      @param dirName : Le nom du dossier
         *  @end
         *
         *  @english
         *      @brief Construtor
         *
         *      This constructor allows to define the name of the directory.
         *      @param dirName : The name of the directory
         *  @end
         */
		Directory(const BString& dirName) : _dirName(dirName)
		{
			_ok = false;
		}

        /*!
         *  @french
         *      @brief Accesseur du chemin d'accès réel.
         *     
         *      Accesseur du chemin d'accès réel
         *      @return Le chemin d'accès réel.
         *  @end
         *
         *  @english
         *      @brief Getter of the real path.
         *
         *      Getter of the real path.
         *      @return The real path to the directory.
         *  @end
         */
		const BString& path() const { return _path; }
        /*!
         *  @french
         *      @brief Mutateur du chemin d'accès réel.
         *
         *      Mutateur du chemin d'accès réel. Ce chemin pointe vers un répertoire dans lequel l'application peut créer de nouveaux fichiers/répertoires.
         *      Le moteur créera automatique le dossier _dirName en cas de besoin.
         *      @param path : Le chemin du répertoire dans lequel le moteur peut écrire.
         *  @end
         *
         *  @english
         *      @brief Setter of real path
         *      Setter of the path of the directory. The path must be a valid path where the application can create files and directory.
         *      When it is needed, the Bean engine will create the directory _dirName at this path.
         *      @param path : The path of the directory.
         *  @end
         */
		void setPath(const BString path)
		{
			_ok = true;
            //if (_dirName != "")
                _path = path + "/" + _dirName;
            //else
             //   _path = path;
		}
        /*!
         *  @french
         *      @brief Verifie la validité du répertoire.
         *
         *      Cette méthode verifie que le répertoire est valide.
         *      @return True si le chemin est valide sinon false
         *  @end
         *
         *  @english
         *      @brief Check the validity of the directory.
         *
         *      Check the validity of the directory.
         *      @return True if the directory is valid otherwise false
         *  @end
         */
		bool isValid() const { return _ok; }
        /*!
         *  @french
         *      @brief Accesseur du nom du répertoire.
         *
         *      Accesseur du nom du répertoire.
         *      @return Le nom du répertoire (sans son chemin d'accès).
         *  @end
         *
         *  @english
         *      @brief Getter of the directory name.
         *      @return The name of the directory (without its path).
         *  @end
         */
		const BString& dirName() const { return _dirName; }
	};

	typedef std::map<BString, Directory> DirMap;

private:
	static BApplication* _instance;

protected:
	bool _independent;
	DirMap _dirMap;

public:
	BApplication();
	static BApplication* instance(void* userdate = NULL);
    /*!
     *  @french
     *      @brief Lance la boucle principale de l'application.
     *
     *      Lance l'application. Cette méthode est bloquante jusqu'à ce que l'application quitte.
     *  @end
     *
     *  @english
     *      @brief Launch the application
     *
     *      Lanch the application. This method will quit when the application is stopped
     *  @end
     */
	virtual void run() = 0;
	virtual void onFocus(bool focus) = 0;
	virtual void onDestroy() = 0;
	virtual void onStart() = 0;
	virtual void onPause() = 0;
	virtual void onResume() = 0;
	virtual void onStop() = 0;
	virtual void onDestroyWindow() = 0;
	virtual void onCreateWindow() = 0;
	virtual void onLowMemory() = 0;
	virtual bool processEvents() = 0;
	virtual void stepApplication() = 0;
	virtual void setIndependentMode(bool independentMode);
	virtual bool isIndependent() const;
	virtual const char* getDirectory(const BString& name);
    virtual void notifyCurrentScript(const BString& script) {};

	virtual void exit() = 0;
	//virtual void pause() = 0;

protected:
	bool findEntryInDirMap(const BString& name, BString* key);
};
