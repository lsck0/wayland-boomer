{
  description = "wayland alternative for tsoding's X11 boomer";

  inputs = {
    nixpkgs.url = "https://channels.nixos.org/nixpkgs-unstable/nixexprs.tar.zst";
  };

  outputs = inputs: {
    packages = builtins.mapAttrs (system: pkgs: {
      default = pkgs.stdenv.mkDerivation rec {
        pname = "wayland-boomer";
        version = "0.0.0";
        src = inputs.self;

        nativeBuildInputs = [
          pkgs.clang
          pkgs.glfw
          pkgs.git
        ];

        buildPhase = ''
					make build
				'';

        installPhase = ''
					install -D ${pname} $out/bin/${pname}
				'';
      };
    }) inputs.nixpkgs.legacyPackages;

		devShells = builtins.mapAttrs (system: pkgs: {
			default = pkgs.mkShell {
				packages = [
					pkgs.clang
					pkgs.glfw
					pkgs.grim
				];
			};
		}) inputs.nixpkgs.legacyPackages;

    apps = builtins.mapAttrs (system: pkgs: {
      default = {
        type = "app";
        program = let
            wayland-boomer = inputs.self.packages.${system}.default;
          in
          "${pkgs.writeShellScript "wayland-boomer-grim" ''
							exec ${pkgs.grim}/bin/grim -t ppm - \
							| ${wayland-boomer}/bin/wayland-boomer > log.txt
						''
					}";
      };
    }) inputs.nixpkgs.legacyPackages;

  };
}
